// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#include "doctest.h"
#include <vector>
#include <cleantype/cleantype.hpp>



#ifdef _MSC_VER
#define DISABLE_TEST
#endif

#if !defined(__clang__) && ( defined(__GNUC__) || defined(__GNUG__) )
    #define IS_GCC_COMPILER
    #define DISABLE_TEST
#endif
#if defined(__clang__) && (__clang_major__ < 7)
    #define DISABLE_TEST
#endif


#ifndef DISABLE_TEST

auto my_make_range(int nb) // return std::vector<int>, but this is deduced
{
    std::vector<int> v;
    for (int i = 0; i < nb; i++)
        v.push_back(i);
    return v;
}

auto my_add(int a, int b)
{
    return a + b;
}

auto foo(int a, int b, int c, int d, int e) // return std::map<std::string, int>, but this is deduced
{
    return std::map<std::string, int> {
      { "1", a },
      { "2", b },
      { "3", c },
      { "4", d },
      { "5", e },
    };
}


TEST_CASE("log_wrap_function")
{
    REQUIRE_EQ(
      show_type_function_fromparams_1(my_make_range, 1),
      "function: (int) -> std::vector<int>"
    );

    // FIXME
    // REQUIRE_EQ(
    //   show_type_function_fromparams_2(my_make_range, 1, 1),
    //   "function: (int) -> std::vector<int>"
    // );

    // REQUIRE_EQ(
    //   show_type_function_fromparams_5(my_make_range, 1, 2, 3, 4, 5),
    //   "function: (int) -> std::vector<int>"
    // );
}

#endif // #ifndef DISABLE_TEST
