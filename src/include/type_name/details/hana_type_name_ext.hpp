#include <cstddef>
#include <utility>
#include <cstring>
#include <boost/hana/config.hpp>
#include <boost/hana/string.hpp>


#ifdef _MSC_VER
#define _TNS__PRETTY_FUNCTION__ __FUNCSIG__
#else
#define _TNS__PRETTY_FUNCTION__ __PRETTY_FUNCTION__
#endif


#if defined(__clang__)
    #define _TNS_PRETTY_FUNCTION_TYPE_PREFIX "boost::hana::hana_type_ext::detail::cstring boost::hana::hana_type_ext::detail::type_name_impl2() [T = "
    #define _TNS_PRETTY_FUNCTION_TYPE_SUFFIX "]"
#elif defined(_MSC_VER)
    #define _TNS_PRETTY_FUNCTION_TYPE_PREFIX  "struct boost::hana::hana_type_ext::detail::cstring __cdecl boost::hana::hana_type_ext::detail::type_name_impl2<"
#define _TNS_PRETTY_FUNCTION_TYPE_SUFFIX ">(void)"
#else // GCC
    #define _TNS_PRETTY_FUNCTION_TYPE_PREFIX "constexpr boost::hana::hana_type_ext::detail::cstring boost::hana::hana_type_ext::detail::type_name_impl2() [with T = "
    #define _TNS_PRETTY_FUNCTION_TYPE_SUFFIX "]"
#endif


// only clang and MSVC support constexpr typename
#if defined(__clang__) || defined(_MSC_VER)
    #define _TNS_CAN_CONSTEXPR
#endif


#ifdef _TNS_CAN_CONSTEXPR
    #define _TNS_CONSTEXPR_IF_POSSIBLE constexpr
    #define _HANA_SIZEOF_OR_STRLEN(var) sizeof(var) - 1
#else
    #define _TNS_CONSTEXPR_IF_POSSIBLE
    #define _HANA_SIZEOF_OR_STRLEN(var) strlen(var)
#endif


BOOST_HANA_NAMESPACE_BEGIN  namespace hana_type_ext {
    namespace detail {
        struct cstring {
            char const* ptr;
            std::size_t length;
        };

        template <typename T>
        constexpr cstring type_name_impl2() {
            _TNS_CONSTEXPR_IF_POSSIBLE char const* pretty_function = _TNS__PRETTY_FUNCTION__;
            _TNS_CONSTEXPR_IF_POSSIBLE std::size_t total_size = _HANA_SIZEOF_OR_STRLEN(_TNS__PRETTY_FUNCTION__);
            _TNS_CONSTEXPR_IF_POSSIBLE std::size_t prefix_size = _HANA_SIZEOF_OR_STRLEN(_TNS_PRETTY_FUNCTION_TYPE_PREFIX);
            _TNS_CONSTEXPR_IF_POSSIBLE std::size_t suffix_size = _HANA_SIZEOF_OR_STRLEN(_TNS_PRETTY_FUNCTION_TYPE_SUFFIX);
            return {pretty_function + prefix_size, total_size - prefix_size - suffix_size};
        }

        template <typename T, std::size_t ...i>
        auto type_name_impl1(std::index_sequence<i...>) {
            constexpr auto name = detail::type_name_impl2<T>();
            auto r = boost::hana::string<*(name.ptr + i)...>{};
            auto r_trim = hana_tn_utils::trim_string_space(r);
            return r_trim;
        }
    } // end namespace detail

    template <typename T>
    auto type_name() {
        #ifdef _TNS_CAN_CONSTEXPR
            constexpr auto name = detail::type_name_impl2<T>();
            return detail::type_name_impl1<T>(std::make_index_sequence<name.length>{});
        #else
            auto as_cstring = detail::type_name_impl2<T>();
            std::string name_full = std::string(as_cstring.ptr);
            std::string name_cut = name_full.substr(0, as_cstring.length);
            return name_cut;
        #endif
    }
} BOOST_HANA_NAMESPACE_END // hana_type_ext
