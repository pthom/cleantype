#pragma once

#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>
#include <string>
#include <type_name/details/fp_polyfill/fp_polyfill.hpp>

#include <boost/hana.hpp>
//#include <boost/hana/experimental/type_name.hpp>

//#define LOG_VAR(var) std::cout << "LOG_VAR " << #var << "==>" << var << "<==" << std::endl;
#define LOG_VAR(var)

#include <boost/hana/config.hpp>
#include <boost/hana/string.hpp>


#ifdef _MSC_VER 
#define _TNS__PRETTY_FUNCTION__ __FUNCSIG__
#else
#define _TNS__PRETTY_FUNCTION__ __PRETTY_FUNCTION__
#endif


#if defined(__clang__)
    #define _TNS_PRETTY_FUNCTION_TYPE_PREFIX "boost::hana::hana_type_copy::detail::cstring boost::hana::hana_type_copy::detail::type_name_impl2() [T = "
    #define _TNS_PRETTY_FUNCTION_TYPE_SUFFIX "]"
#elif defined(_MSC_VER)
    #define _TNS_PRETTY_FUNCTION_TYPE_PREFIX  "struct boost::hana::hana_type_copy::detail::cstring __cdecl boost::hana::hana_type_copy::detail::type_name_impl2<"
#define _TNS_PRETTY_FUNCTION_TYPE_SUFFIX ">(void)"
#else // GCC
    #define _TNS_PRETTY_FUNCTION_TYPE_PREFIX "constexpr boost::hana::hana_type_copy::detail::cstring boost::hana::hana_type_copy::detail::type_name_impl2() [with T = "
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
BOOST_HANA_NAMESPACE_BEGIN  namespace hana_type_copy {
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
            LOG_VAR(pretty_function);
            LOG_VAR(_TNS_PRETTY_FUNCTION_TYPE_PREFIX);
            LOG_VAR(total_size);
            LOG_VAR(prefix_size);
            LOG_VAR(suffix_size);
            LOG_VAR(pretty_function + prefix_size);
            return {pretty_function + prefix_size, total_size - prefix_size - suffix_size};
            //return {pretty_function, total_size};
        }

        template <typename T, std::size_t ...i>
        auto type_name_impl1(std::index_sequence<i...>) {
            constexpr auto name = detail::type_name_impl2<T>();
            return boost::hana::string<*(name.ptr + i)...>{};
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
            LOG_VAR(as_cstring.ptr);
            LOG_VAR(as_cstring.length);
            LOG_VAR(name_full);
            std::string name_cut = name_full.substr(0, as_cstring.length);
            LOG_VAR(name_cut);
            return name_cut;
        #endif
    }
} BOOST_HANA_NAMESPACE_END // hana_type_copy



namespace type_name_s
{
    namespace internal
    {
        inline std::string remove_spaces_before(const char token, const std::string & str)
        {
            std::string result;
            bool space_before = false;
            for (auto c : str)
            {
                if ( (c == token) && space_before )
                {
                    result.pop_back();
                }
                result = result + c;

                if (c == ' ')
                    space_before = true;
                else
                    space_before = false;

            }
            return result;
        }


        inline std::string insert_spaces_before(const char token, const std::string & str)
        {
            std::string result;
            bool space_or_same_token_before = true;
            for (auto c : str)
            {
                if ((c == token) && !(space_or_same_token_before))
                    result = result + " ";
                result = result + c;
                if ((c == ' ') || (c == token))
                    space_or_same_token_before = true;
                else
                    space_or_same_token_before = false;
            }
            return result;
        }

        inline std::string insert_spaces_after(const char token, const std::string & str)
        {
            std::string result;
            bool token_before = false;
            for (auto c : str)
            {
                if (token_before && (c != ' '))
                    result = result + ' ';
                result = result + c;
                token_before = (c == token);
            }
            return result;
        }

        inline std::string insert_spaces_before_after(const char token, const std::string & str)
        {
            std::string result = insert_spaces_before(token, str);
            result = insert_spaces_after(token, result);
            return result;
        }


        template<typename T>
        auto impl_typeid_native()
        {
            typedef typename std::remove_reference<T>::type TR;
            std::unique_ptr<char, void(*)(void*)> own
                (
        #ifndef _MSC_VER
                        abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                                nullptr, nullptr),
        #else
                        nullptr,
        #endif
                        std::free
                );
            std::string r = own != nullptr ? own.get() : typeid(TR).name();
            if (std::is_const<TR>::value)
                r =  r + " const";
            if (std::is_volatile<TR>::value)
                r = r + " volatile" ;
            if (std::is_lvalue_reference<T>::value)
                r += "&";
            else if (std::is_rvalue_reference<T>::value)
                r += "&&";
            return r;
        }


        template<typename T>
        std::string impl_typeid_hana()
        {
            // will make the compiler fail (no call operator), but you can read
            // the name in the output if you squint your eyes
            //constexpr auto t = boost::hana::experimental::type_name<T>()();
            std::string r =  boost::hana::hana_type_copy::type_name<T>().c_str();
            std::string r_trim = fp::trim(' ', r);
            return r_trim;
        }


        template<typename T>
        auto impl_typeid()
        {
            return impl_typeid_hana<T>();
        }


        template<typename T>
        auto impl_typeid_recursive()
        {
            return impl_typeid<T>();  //std::string("");
        }
        template <typename First, typename Second, typename ...Rest>
        std::string impl_typeid_recursive () {
            return impl_typeid_recursive<First>() + ", " + impl_typeid_recursive<Second, Rest...>();
        }



        template <typename... T> std::string impl_full()
        {
            std::string r = impl_typeid_recursive<T...>().c_str();
            r = insert_spaces_after(',', r);
            r = insert_spaces_before_after('&', r);
            r = insert_spaces_before_after('*', r);
            r = insert_spaces_before_after('[', r);
            r = fp::replace_tokens("*&", "* &", r);
            r = fp::replace_tokens("&*", "& *", r);
            r = fp::replace_tokens("& &", "&&", r);
            r = fp::replace_tokens(" ,", ",", r);
            return r;
        }
    } // namespace internal


    template <typename... T>
    std::string full () {
        return type_name_s::internal::impl_full<T...>();
    }
/*
    template <typename First, typename Second, typename ...Rest>
    std::string full () {
        return type_name_s::internal::impl_full<First>()+ ", " + full<Second, Rest...>();
    }
*/


    template <typename... T> std::string full(T... v) {
        return internal::impl_full<T...>();
    }
//    template <typename First, typename Second, typename ...Rest>
//    std::string full(First && first, Second && second, Rest... rest) {
//        return internal::impl_full<First>() + ", " + full<Second, Rest...>(
//            std::forward<Second>(second),
//            std::forward<Rest>(rest)...
//        );
//    }


    template <typename T> std::string show_details_full(T && v) {
        return std::string("[") + full<T>() + "]" + " = " + fp::show(v);
    }
} // namespace type_name


#define TNS_type_name_full(var) type_name_s::full<decltype(var)>()

#define TNS_show_details_full(var) \
        std::string("[") + TNS_type_name_full(var) + "] " + #var \
        + " = " \
        + fp::show(var)

#define TNS_show_details_full_cont(var) \
        std::string("[") + TN_type_name_full(var) + "] " + #var \
        + " = " \
        + fp::show_cont(var)
