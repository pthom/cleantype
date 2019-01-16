// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once
#include <cleantype/details/cleantype_clean_impl.hpp>
#include <cleantype/details/cleantype_full.hpp>
#include <deque>
#include <string>

namespace cleantype
{
    inline std::string clean_typestring(const std::string & type_names)
    {
        return internal::impl_clean(type_names);
    }

    inline std::string indent_type_tree(const std::string & type_names)
    {
        return internal::impl_indent_type_tree(type_names);
    }

    template <typename... T>
    std::string clean()
    {
        return clean_typestring(internal::impl_full<T...>());
    }

    template <typename... T>
    std::string clean(T &&... v)
    {
        return clean<T...>();
    }

    template <class T>
    std::string show_details(T && v)
    {
        return clean<T>() + " = " + cleantype_fp::show(v);
    }

}  // namespace cleantype

#define CT_cleantype_clean(var) cleantype::clean<decltype(var)>()

#define CT_show_details(var) \
    std::string("[") + CT_cleantype_clean(var) + "] " + #var + " = " + cleantype_fp::show(var)

#define CT_show_details_cont(var) \
    std::string("[") + CT_cleantype_clean(var) + "] " + #var + " = " + cleantype_fp::show_cont(var)
