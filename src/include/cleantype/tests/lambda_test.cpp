// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#include "doctest.h"
#include <cleantype/cleantype.hpp>
#include <cleantype/details/debug_break.hpp>
#include <fplus/fplus.hpp>
#include <functional>
#include <map>

#define LOG(str) std::cout << str << std::endl

template<typename Lambda> void test_one_lambda(Lambda f, std::string const & expected_type)
{
    const std::string computed_type = CT_show_details_lambda(f);
    if (computed_type != expected_type)
        DEBUGBREAK;
    REQUIRE_EQ(computed_type, expected_type);
}



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
