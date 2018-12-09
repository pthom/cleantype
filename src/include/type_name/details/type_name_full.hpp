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
                r = "volatile" + r;
            if (std::is_lvalue_reference<T>::value)
                r += "&";
            else if (std::is_rvalue_reference<T>::value)
                r += "&&";
            return r;
        }
    } // namespace internal


    template <class T> std::string full() {
        return internal::impl_full<T>();
    }


    template <class T> std::string show_details_full(T && v) {
        return std::string("[") + full<T>() + "]" + " = " + fp::show(v);
    }


    //////////////////////////////
    // Start of public API
    //////////////////////////////

    // * `type_name::full<T>()` is a function that will return a string containing
    //    the full type of a variable.
    template <class T> std::string full();
    // * `type_name::show_details_full(T && v)` is a function that will return a string containing
    //    the full type of a variable, as well as its content
    template <class T> std::string show_details_full(T && v);
} // namespace type_name


// * `tn_type_name_full(var)` is a macro that will also return the full type,
//    but, it is able to also correctly display rvalue reference types.
#define tn_type_name_full(var) type_name::full<decltype(var)>()

// *  `tn_show_details_full(var)` will return a string containing the name,
//    type and content of a variable (in this case, the underlying type of 'var'
//    has to have an 'ostream & operator<<')
#define tn_show_details_full(var) \
        std::string("[") + tn_type_name_full(var) + "] " + #var \
        + " = " \
        + fp::show(var)

// * `tn_show_details_full_cont` is a version of tn_show_details_full for complex containers
//    like "std::map". "cont" stands for "container".
#define tn_show_details_full_cont(var) \
        std::string("[") + tn_type_name_full(var) + "] " + #var \
        + " = " \
        + fp::show_cont(var)
