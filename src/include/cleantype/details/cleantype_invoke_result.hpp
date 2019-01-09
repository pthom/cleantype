// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once

#include <cleantype/details/cleantype_full.hpp>
#include <cleantype/details/cleantype_clean.hpp>
#include <cleantype/details/invoke_result_polyfill.hpp>

namespace cleantype
{
    #if __cplusplus < 201703L
    template <class F, class... ArgTypes>
    struct invoke_result : detail::invoke_result<void, F, ArgTypes...> {};
    #else
    template <class F, class... ArgTypes>
    struct invoke_result : std::invoke_result<F, ArgTypes...> {};
    #endif


    #define CT_invoke_result_fn(f, ...) \
    cleantype::invoke_result<decltype(&f) , __VA_ARGS__>::type

    #define CT_invoke_result_fn_template(f, ...) \
    cleantype::invoke_result<decltype(&f<__VA_ARGS__>) , __VA_ARGS__>::type


    template <class F, class... ArgTypes>
    inline std::string invoke_result_clean()
    {
        using T = typename cleantype::invoke_result<F, ArgTypes...>::type;
        return cleantype::clean<T>();
    }

    template <class F, class... ArgTypes>
    inline std::string invoke_result_full()
    {
        using T = typename cleantype::invoke_result<F, ArgTypes...>::type;
        return cleantype::full<T>();
    }
}