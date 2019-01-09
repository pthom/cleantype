// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#include "doctest.h"
#include <vector>
#include <cleantype/details/invoke_result_polyfill.hpp>


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
