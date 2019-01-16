// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#include "doctest.h"
#include <cleantype/cleantype.hpp>
#include <cleantype/details/cleantype_invoke_result.hpp>
#include <vector>

// #define LOG(v) std::cout << v << "\n";
// #define LOGVAR(v) std::cout << #v << " ==>" << v << "<==\n";

auto add_auto_fn(int a, int b) { return a + b; }

template <typename U, typename V>
auto add_auto_template_fn(U a, V b)
{
    return a + b;
}

int fortytwo(int a, int b) { return a + 42; }

struct my_functor
{
    auto operator()(int a) { return a + 42; }
};

TEST_CASE("test_invoke_result")
{
    {
        // For functions, use < decltype(&f), Args... >
        using T = cleantype::invoke_result<decltype(&fortytwo), int, int>::type;
        static_assert(std::is_same<T, int>::value, "");
        using U = cleantype::invoke_result_t<decltype(&fortytwo), int, int>;
        static_assert(std::is_same<U, int>::value, "");
    }
    {
        // For auto functions, use < decltype(&f), Args... >
        using T = cleantype::invoke_result<decltype(&add_auto_fn), int, int>::type;
        static_assert(std::is_same<T, int>::value, "");
        using U = cleantype::invoke_result_t<decltype(&add_auto_fn), int, int>;
        static_assert(std::is_same<U, int>::value, "");
    }
    {
        // For templated auto functions: use < decltype(&f)<Args...>, Args... >
        // this does *not* work under MSVC, because of a probable compiler bug
        // See
        // https://stackoverflow.com/questions/54111146/invoke-result-for-template-function-with-auto-return-type-and-msvc-2017
#ifndef _MSC_VER
        using T = cleantype::
            invoke_result<decltype(&add_auto_template_fn<int, double>), int, double>::type;
        static_assert(std::is_same<T, double>::value, "");
        using U =
            cleantype::invoke_result_t<decltype(&add_auto_template_fn<int, double>), int, double>;
        static_assert(std::is_same<U, double>::value, "");
#endif
    }
    {
        // For simple lambdas: use < decltype(lambda), Args... >
        auto simple_lambda = [](int a) { return a + 42; };
        using T = cleantype::invoke_result<decltype(simple_lambda), int>::type;
        static_assert(std::is_same<T, int>::value, "");
        using U = cleantype::invoke_result_t<decltype(simple_lambda), int>;
        static_assert(std::is_same<U, int>::value, "");
    }
    {
        // For generic lambdas: use < decltype(lambda), Args... >
        auto generic_lambda = [](auto a) { return a + 42; };
        using T = cleantype::invoke_result<decltype(generic_lambda), double>::type;
        static_assert(std::is_same<T, double>::value, "");
        using U = cleantype::invoke_result_t<decltype(generic_lambda), double>;
        static_assert(std::is_same<U, double>::value, "");
    }
    {
        // For functors: use < functor, Args... >
        using T = cleantype::invoke_result<my_functor, int>::type;
        static_assert(std::is_same<T, int>::value, "");
        using U = cleantype::invoke_result_t<my_functor, int>;
        static_assert(std::is_same<U, int>::value, "");
    }
}

TEST_CASE("test_invoke_result_FN")
{
    {
        // For functions and auto function: use CT_invoke_result_fn(f, Args...)
        using T = CT_invoke_result_fn(fortytwo, int, int);
        static_assert(std::is_same<T, int>::value, "");

        REQUIRE_EQ(CT_type_fn(fortytwo, int, int), "int");
    }
#ifndef _MSC_VER
    {
        // For templated auto functions: use CT_invoke_result_fn_template(f, Args...)
        using T = CT_invoke_result_fn_template(add_auto_template_fn, int, double);
        static_assert(std::is_same<T, double>::value, "");

        REQUIRE_EQ(CT_type_fn_template(add_auto_template_fn, int, double), "double");
    }
#else
    {
        // For templated auto functions under MSVC:
        // use CT_invoke_result_fn_template(f, Args...)
        // you will need to "help" the compiler before, by calling
        // CT_invoke_result_fn_template_memoize
        CT_invoke_result_fn_template_memoize(add_auto_template_fn, int, double);
        using T = CT_invoke_result_fn_template(add_auto_template_fn, int, double);
        static_assert(std::is_same<T, double>::value, "");
    }
#endif
}
