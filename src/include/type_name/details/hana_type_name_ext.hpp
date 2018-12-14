#include <boost/hana/config.hpp>
#include <boost/hana/string.hpp>


// Used only to help debug
//#define TNS_LOG_VAR(var) std::cout << "LOG_VAR " << #var << "==>" << var << "<==" << std::endl;
#define TNS_LOG_VAR(var)


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
    #define hana_type_copy_sizeof(var) sizeof(var) - 1
#else
    #define _TNS_CONSTEXPR_IF_POSSIBLE
    #define hana_type_copy_sizeof(var) strlen(var)
#endif

#include <cstddef>
#include <utility>
#include <cstring>
BOOST_HANA_NAMESPACE_BEGIN  namespace hana_type_ext {
    namespace detail {
        struct cstring {
            char const* ptr;
            std::size_t length;
        };

        // Note: We substract the null terminator from the string sizes below.
        template <typename T>
        constexpr cstring type_name_impl2() {
            _TNS_CONSTEXPR_IF_POSSIBLE char const* pretty_function = _TNS__PRETTY_FUNCTION__;
            _TNS_CONSTEXPR_IF_POSSIBLE std::size_t total_size = hana_type_copy_sizeof(_TNS__PRETTY_FUNCTION__);
            _TNS_CONSTEXPR_IF_POSSIBLE std::size_t prefix_size = hana_type_copy_sizeof(_TNS_PRETTY_FUNCTION_TYPE_PREFIX);
            _TNS_CONSTEXPR_IF_POSSIBLE std::size_t suffix_size = hana_type_copy_sizeof(_TNS_PRETTY_FUNCTION_TYPE_SUFFIX);
            // TNS_LOG_VAR(pretty_function);
            // TNS_LOG_VAR(_TNS_PRETTY_FUNCTION_TYPE_PREFIX);
            // TNS_LOG_VAR(total_size);
            // TNS_LOG_VAR(prefix_size);
            // TNS_LOG_VAR(suffix_size);
            // TNS_LOG_VAR(pretty_function + prefix_size);
            return {pretty_function + prefix_size, total_size - prefix_size - suffix_size};
            //return {pretty_function, total_size};
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
            // TNS_LOG_VAR(as_cstring.ptr);
            // TNS_LOG_VAR(as_cstring.length);
            // TNS_LOG_VAR(name_full);
            std::string name_cut = name_full.substr(0, as_cstring.length);
            // TNS_LOG_VAR(name_cut);
            return name_cut;
        #endif
    }
} BOOST_HANA_NAMESPACE_END // hana_type_ext
