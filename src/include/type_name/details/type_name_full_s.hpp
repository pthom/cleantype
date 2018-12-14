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
#include "type_name/details/hana_type_name_ext.hpp"
//#include <boost/hana/experimental/type_name.hpp>


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
            return boost::hana::hana_type_ext::type_name<T>();
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
         static_assert(boost::hana::hana_type_ext::type_name<T>(), "truc");
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
