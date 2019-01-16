// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once

#include <cleantype/details/cleantype_clean.hpp>
#include <cleantype/details/cleantype_full.hpp>
#include <cleantype/details/invoke_result_polyfill.hpp>

namespace cleantype
{
#if __cplusplus < 201703L
    template <class F, class... ArgTypes>
    struct invoke_result : detail::invoke_result<void, F, ArgTypes...>
    {
    };
#else
    template <class F, class... ArgTypes>
    struct invoke_result : std::invoke_result<F, ArgTypes...>
    {
    };
#endif

    template <class F, class... ArgTypes>
    using invoke_result_t = typename invoke_result<F, ArgTypes...>::type;
}  // namespace cleantype

#define CT_invoke_result_fn(f, ...) cleantype::invoke_result<decltype(&f), __VA_ARGS__>::type

#define CT_invoke_result_fn_template(f, ...) \
    cleantype::invoke_result<decltype(&f<__VA_ARGS__>), __VA_ARGS__>::type

#define CT_type_fn(f, ...) cleantype::clean<CT_invoke_result_fn(f, __VA_ARGS__)>()
#define CT_type_fn_full(f, ...) cleantype::full<CT_invoke_result_fn(f, __VA_ARGS__)>()

#define CT_type_fn_template(f, ...) cleantype::clean<CT_invoke_result_fn_template(f, __VA_ARGS__)>()
#define CT_type_fn_template_full(f, ...) \
    cleantype::full<CT_invoke_result_fn_template(f, __VA_ARGS__)>()

#ifdef _MSC_VER
// under MSVC 2017, std::invoke_result fails with template functions
// See
// https://stackoverflow.com/questions/54111146/invoke-result-for-template-function-with-auto-return-type-and-msvc-2017
// You need to call CT_invoke_result_fn_template_memoize before calling CT_invoke_result_fn_template
// in order to help the compiler
#define CT_invoke_result_fn_template_memoize(f, ...) \
    {                                                \
        auto _ = f<__VA_ARGS__>;                     \
        (void)_;                                     \
    }
#endif
