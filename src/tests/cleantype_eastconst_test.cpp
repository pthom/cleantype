// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#include "doctest.h"
#include <cleantype/cleantype.hpp>
#include <cleantype/details/debug_utilities.hpp>

#ifdef _WIN32
#include <Windows.h>
#endif

template <typename Transform>
auto make_test_string_transform(Transform f)
{
    return [f](std::string const & input, std::string const & expected_output) {
        std::string computed_output = f(input);
        if (computed_output != expected_output)
            DEBUGBREAK;
        REQUIRE_EQ(computed_output, expected_output);
    };
}

TEST_CASE("apply_east_const_typelist")
{
    auto make_one_test = make_test_string_transform(cleantype::apply_east_const_typelist);

    // T => T
    make_one_test("T", "T");
    // T * => T *
    make_one_test("T *", "T *");
    make_one_test("T * &", "T * &");

    // const T => T const
    make_one_test("T const", "T const");
    make_one_test("const T", "T const");

    // const T & => T const &
    make_one_test("T const &", "T const &");
    make_one_test("const T &", "T const &");

    // const * T => T const *
    make_one_test("T const *", "T const *");
    make_one_test("const T *", "T const *");

    // const T * const => T const * const
    make_one_test("T const * const", "T const * const");
    make_one_test("const T * const", "T const * const");

    // const T * & => T const * &
    make_one_test("T const * &", "T const * &");
    make_one_test("const T * &", "T const * &");

    // const T * const & => T const * const &
    make_one_test("const int * const &", "int const * const &");
}
