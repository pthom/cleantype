// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#include "doctest.h"
#include <vector>
#include <cleantype/details/cleantype_invoke_result.hpp>
#include <cleantype/cleantype.hpp>


// #define LOG(v) std::cout << v << "\n";
// #define LOGVAR(v) std::cout << #v << " ==>" << v << "<==\n";

auto add_auto_fn(int a, int b) {
    return a + b;
}

template<typename U, typename V>
auto add_auto_template_fn(U a, V b) {
    return a + b;
}

int fortytwo(int a, int b) { return a + 42; }

struct my_functor{
    auto operator() (int a) { return a + 42; }
};


void static_test_invoke_result()
{
    {
        // For functions and auto function: use < decltype(&f), Args... >
        using T = cleantype::invoke_result< decltype(&fortytwo), int, int>::type;
        static_assert(std::is_same<T, int>::value, "");
    }
    {
        // For templated auto functions: use < decltype(&f)<Args...>, Args... >
        using T = cleantype::invoke_result< decltype(&add_auto_template_fn<int, double>), int, double>::type;
        static_assert(std::is_same<T, double>::value, "");
    }
    {
        // For simple lambdas: use < decltype(lambda), Args... >
        auto simple_lambda = [](int a) {  return a + 42; };
        using T = cleantype::invoke_result< decltype(simple_lambda), int>::type;
        static_assert(std::is_same<T, int>::value, "");
    }
    {
        // For generic lambdas: use < decltype(lambda), Args... >
        auto generic_lambda = [](auto a) {  return a + 42; };
        using T = cleantype::invoke_result< decltype(generic_lambda), double>::type;
        static_assert(std::is_same<T, double>::value, "");
    }
    {
        // For functors: use < functor, Args... >
        using T = cleantype::invoke_result< my_functor, int>::type;
        static_assert(std::is_same<T, int>::value, "");
    }
}


void static_test_invoke_result_fn()
{
    {
        // For functions and auto function: use CT_invoke_result_fn(f, Args...)
        using T = CT_invoke_result_fn(fortytwo, int, int);
        static_assert(std::is_same<T, int>::value, "");
    }
    {
        // For templated auto functions: use CT_invoke_result_fn_template(f, Args...)
        using T = CT_invoke_result_fn_template(add_auto_template_fn, int, double);
        static_assert(std::is_same<T, double>::value, "");
    }

}



TEST_CASE("invoke_result_clean")
{
    {
        // For functions and auto function: use < decltype(&f), Args... >
        REQUIRE_EQ(
            cleantype::invoke_result_clean<decltype(&fortytwo), int, int>(),
            "int"
        );
    }
    {
        // For templated auto functions: use < decltype(&f)<Args...>, Args... >
        REQUIRE_EQ(
            cleantype::invoke_result_clean<
                decltype(&add_auto_template_fn<std::string, char>),
                std::string, char>
                (),
            "std::string"
        );
    }
    {
        // For simple lambdas: use < decltype(lambda), Args... >
        auto simple_lambda = [](int a) {  return a + 42; };
        REQUIRE_EQ(
            cleantype::invoke_result_clean< decltype(simple_lambda), int >(),
            "int"
        );
    }
    {
        // For generic lambdas: use < decltype(lambda), Args... >
        auto generic_lambda = [](auto a) {  return a + "=42"; };
        REQUIRE_EQ(
            cleantype::invoke_result_clean< decltype(generic_lambda), std::string>(),
            "std::string"
        );
    }
    {
        // For functors: use < functor, Args... >
        REQUIRE_EQ(
            cleantype::invoke_result_clean< my_functor, int>(),
            "int"
        );
    }
}
