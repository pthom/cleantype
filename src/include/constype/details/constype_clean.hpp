// This file is part of ConsType : Consistent Type names for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once
#include <string>
#include <deque>
#include <constype/details/constype_full.hpp>
#include <constype/details/constype_clean_impl.hpp>

namespace constype
{
    template <typename... T>
    std::string clean()
    {
        return internal::impl_clean_several_types(internal::impl_full<T...>());
    }

    template <typename... T> std::string clean(T&&... v) {
        return clean<T...>();
    }

    template <class T> std::string show_details(T && v) {
        return clean<T>() + " = " + fp::show(v);
    }

} // namespace constype


#define TN_constype_clean(var) constype::clean<decltype(var)>()


#define TN_show_details(var) \
        std::string("[") + TN_constype_clean(var) + "] " + #var \
        + " = " \
        + fp::show(var)


#define TN_show_details_cont(var) \
        std::string("[") + TN_constype_clean(var) + "] " + #var \
        + " = " \
        + fp::show_cont(var)
