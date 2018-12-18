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
#include <type_name/details/hana_ext/experimental/type_name.hpp>
#include <type_name/details/type_name_format_whitespace.hpp>

namespace type_name_s
{
    namespace internal
    {
        template<typename T>
        auto impl_typeid_hana()
        {
            return boost::hana::experimental::type_name<T>();
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
            #ifdef _HANA_TN_CAN_CONSTEXPR
                return impl_typeid_recursive<First>() + BOOST_HANA_STRING(", ") + impl_typeid_recursive<Second, Rest...>();
            #else
                return impl_typeid_recursive<First>() + std::string(", ") + impl_typeid_recursive<Second, Rest...>();
            #endif
        }


        template <typename... T> std::string impl_full()
        {
            std::string r = impl_typeid_recursive<T...>().c_str();
            std::string formatted = type_name::format_whitespace(r);
            return formatted;
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
         static_assert(boost::hana::experimental::type_name<T>(), "truc");
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
