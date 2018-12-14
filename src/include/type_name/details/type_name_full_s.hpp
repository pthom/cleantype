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
#include "type_name/details/hana_utils/hana_string_utils.hpp"
#include <type_name/details/fp_polyfill/fp_polyfill.hpp>

#include <boost/hana.hpp>
//#include <boost/hana/experimental/type_name.hpp>

//#define TNS_LOG_VAR(var) std::cout << "LOG_VAR " << #var << "==>" << var << "<==" << std::endl;
#define TNS_LOG_VAR(var)

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
            TNS_LOG_VAR(pretty_function);
            TNS_LOG_VAR(_TNS_PRETTY_FUNCTION_TYPE_PREFIX);
            TNS_LOG_VAR(total_size);
            TNS_LOG_VAR(prefix_size);
            TNS_LOG_VAR(suffix_size);
            TNS_LOG_VAR(pretty_function + prefix_size);
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
            TNS_LOG_VAR(as_cstring.ptr);
            TNS_LOG_VAR(as_cstring.length);
            TNS_LOG_VAR(name_full);
            std::string name_cut = name_full.substr(0, as_cstring.length);
            TNS_LOG_VAR(name_cut);
            return name_cut;
        #endif
    }
} BOOST_HANA_NAMESPACE_END // hana_type_copy


namespace type_name_s
{
    namespace internal
    {
        // TODO : port to hana::string !
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


        // TODO : port to hana::string !
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


        // TODO : port to hana::string !
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


        // TODO : port to hana::string !
        inline std::string insert_spaces_before_after(const char token, const std::string & str)
        {
            std::string result = insert_spaces_before(token, str);
            result = insert_spaces_after(token, result);
            return result;
        }


        template<typename T>
        auto impl_typeid_hana()
        {
            return boost::hana::hana_type_copy::type_name<T>();
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
        auto impl_typeid_recursive () {
            #ifdef _TNS_CAN_CONSTEXPR
                return impl_typeid_recursive<First>() + BOOST_HANA_STRING(", ") + impl_typeid_recursive<Second, Rest...>();
            #else
                return impl_typeid_recursive<First>() + std::string(", ") + impl_typeid_recursive<Second, Rest...>();
            #endif
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


    template <typename... T> std::string full(T... v) {
        return internal::impl_full<T...>();
    }


    template <typename T> std::string show_details_full(T && v) {
        return std::string("[") + full<T>() + "]" + " = " + fp::show(v);
    }


     template<typename T>
     void ERROR_full() {
       // will make the compiler fail (no call operator), but you can read
       // the name in the output if you squint your eyes
       //constexpr auto t = boost::hana::experimental::type_name<T>()();
       //static_assert(internal::impl_full<T...>() , "truc");
         static_assert(boost::hana::hana_type_copy::type_name<T>(), "truc");
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
