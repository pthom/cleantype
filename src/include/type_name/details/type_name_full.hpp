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
#include <type_name/details/type_name_format_whitespace.hpp>
#include <type_name/details/fp_polyfill/fp_polyfill.hpp>

namespace type_name
{
    namespace internal
    {
        template <class T> std::string impl_full()
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

            r = type_name::format_whitespace(r);
            return r;
        }
    } // namespace internal


    template <typename T>
    std::string full () {
        return internal::impl_full<T>();
    }
    template <typename First, typename Second, typename ...Rest>
    std::string full () {
        return internal::impl_full<First>()+ ", " + full<Second, Rest...>();
    }


    template <typename T> std::string full(T && v) {
        return internal::impl_full<T>();
    }
    template <typename First, typename Second, typename ...Rest>
    std::string full(First && first, Second && second, Rest... rest) {
        return internal::impl_full<First>() + ", " + full<Second, Rest...>(
            std::forward<Second>(second),
            std::forward<Rest>(rest)...
        );
    }


    template <typename T> std::string show_details_full(T && v) {
        return std::string("[") + full<T>() + "]" + " = " + fp::show(v);
    }
} // namespace type_name


#define TN_type_name_full(var) type_name::full<decltype(var)>()

#define TN_show_details_full(var) \
        std::string("[") + TN_type_name_full(var) + "] " + #var \
        + " = " \
        + fp::show(var)

#define TN_show_details_full_cont(var) \
        std::string("[") + TN_type_name_full(var) + "] " + #var \
        + " = " \
        + fp::show_cont(var)
