// This file is part of ConsType : Consistent Type names for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once
#include <string>
#include <deque>
#include <cleantype/details/cleantype_full.hpp>
#include <cleantype/details/cleantype_clean_impl.hpp>

namespace cleantype
{
    inline std::string clean_typestring(const std::string & type_names)
    {
        return internal::impl_clean_several_types(type_names);
    }

    template <typename... T>
    std::string clean()
    {
        return clean_typestring(internal::impl_full<T...>());
    }

    template <typename... T> std::string clean(T&&... v) {
        return clean<T...>();
    }

    template <class T> std::string show_details(T && v) {
        return clean<T>() + " = " + fp::show(v);
    }

} // namespace cleantype


#define TN_cleantype_clean(var) cleantype::clean<decltype(var)>()


#define TN_show_details(var) \
        std::string("[") + TN_cleantype_clean(var) + "] " + #var \
        + " = " \
        + fp::show(var)


#define TN_show_details_cont(var) \
        std::string("[") + TN_cleantype_clean(var) + "] " + #var \
        + " = " \
        + fp::show_cont(var)
