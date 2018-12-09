#pragma once

#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>

namespace type_name
{
    namespace internal
    {
        template <class T> std::string full()
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


    //////////////////////////////
    // Start of public API
    //////////////////////////////

    template <class T> std::string full() { return internal::full<T>();
} // namespace type_name



#define m_type_name_full(var) type_name::full<decltype(var)>()

#define m_show_details_full(var) \
        std::string("[") + m_type_name_full(var) + "] " + #var \
        + " = " \
        + fp::show(var)

#define m_show_details_full_cont(var) \
        std::string("[") + m_type_name_full(var) + "] " + #var \
        + " = " \
        + fp::show_cont(var)

} // namespace fplus
