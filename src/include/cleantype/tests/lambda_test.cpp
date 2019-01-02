// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#include "doctest.h"
#include <cleantype/cleantype.hpp>
#include <cleantype/details/debug_break.hpp>
#include <fplus/fplus.hpp>
#include <functional>
#include <map>
#include <cmath>

#define LOG(str) std::cout << str << std::endl
#define LOG_VALUE(v) std::cout << #v << "==>" << v << "<==" << std::endl

template<typename Lambda> void test_one_lambda(Lambda f, std::string const & expected_type)
{
    const std::string computed_type = CT_show_details_lambda(f);
    if (computed_type != expected_type)
        DEBUGBREAK;
    REQUIRE_EQ(computed_type, expected_type);
}

#define CT_show_details_lambda2(f) std::string("[") + cleantype::lambda_clean(f) + "] " + #f


TEST_CASE("log_type_lambda_clean")
{
  {
    auto f = []() { std::cout << "Hello"; };
    REQUIRE_EQ(CT_show_details_lambda(f), "[lambda: () -> void] f");
  }
  {
    auto f = []() { return 42u; };
    auto s = CT_show_details_lambda(f);
    test_one_lambda(f, "[lambda: () -> unsigned int] f");
  }
  {
    int c = 5;
    auto f = [&c](int a, int b) -> double { return a + b + c; };
    test_one_lambda(f, "[lambda: (int, int) -> double] f");
  }
  {
      auto f = [](std::string const &a, std::string const & b) -> std::string const { return a + b; };
      test_one_lambda(f, "[lambda: (std::string const &, std::string const &) -> std::string const] f");
  }
  {
    int c = 5;
    auto f = [](int a, int b) { return std::pair<int, double>(a + b, cos(a + static_cast<double>(b))); };
    test_one_lambda(f, "[lambda: (int, int) -> std::pair<int, double>] f");
  }
  {
    std::string prefix = "a-";
    auto f = [&prefix](std::string const &s) { return prefix + s; };
    test_one_lambda(f, "[lambda: (std::string const &) -> std::string] f");
  }
}


TEST_CASE("lambda_generic_or_not")
{
    {
        auto lambda = [](auto v) {
            return v + 42;
        };
        REQUIRE_EQ(cleantype::lambda_full<int>(lambda), "lambda: (int) -> int");
    }
    {
        auto lambda = [](int v) {
            return v + 42;
        };
        REQUIRE_EQ(cleantype::lambda_full(lambda), "lambda: (int) -> int");
    }
}

TEST_CASE("lambda_clean_flag")
{
    {
        auto make_repeat_vector = [](auto v, unsigned int nb) {
            std::vector<decltype(v)> r;
            for (std::size_t i = 0; i < nb; i++)
                r.push_back(v);
            return r;
        };
        REQUIRE_EQ(
            cleantype::lambda<std::string>(make_repeat_vector, true),
            "lambda: (std::string, unsigned int) -> std::vector<std::string>");

        std::string full_type = cleantype::lambda<std::string>(make_repeat_vector, false);
        // This test is disabled : it is very much compiler and stdlib dependant !
        // REQUIRE_EQ(
        //     full_type,
        //     "lambda: (std::__1::basic_string<char>, unsigned int) -> std::__1::vector<std::__1::basic_string<char>, std::__1::allocator<std::__1::basic_string<char> > >");

    }

}