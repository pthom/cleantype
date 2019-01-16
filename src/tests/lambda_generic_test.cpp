// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#include "doctest.h"
#include <cleantype/cleantype.hpp>
#include <fplus/fplus.hpp>
#include <functional>
#include <map>

#define LOG(str) std::cout << str << std::endl

TEST_CASE("lambda_generic_clean")
{
    {
        auto f = [](auto i) { return 2 * i; };
        REQUIRE_EQ(cleantype::lambda_clean<int>(f), "lambda: (int) -> int");
    }
    {
        auto f = [](auto b) {
            std::vector<int> range = fplus::numbers(0, b);
            return fplus::pairs_to_map_grouped(fplus::overlapping_pairs_cyclic(range));
        };
        REQUIRE_EQ(cleantype::lambda_clean<int>(f),
                   "lambda: (int) -> std::map<int, std::vector<int>>");
    }
    {
        auto f = [](auto a, auto b) { return a + b; };
        REQUIRE_EQ(cleantype::lambda_clean<int, char>(f), "lambda: (int, char) -> int");
    }
}

TEST_CASE("lambda_generic_1_to_5_params")
{
    {
        auto f = [](auto i) { return 2 * i; };
        REQUIRE_EQ(CT_type_lambda_generic_fromparams_1(f, 1), "lambda: (int) -> int");
    }
    {
        auto f = [](auto i, auto j) { return 2 * i + j; };
        REQUIRE_EQ(CT_type_lambda_generic_fromparams_2(f, 1, 1), "lambda: (int, int) -> int");
    }
    {
        auto f = [](auto i, auto j, auto k) { return 2 * i + j + 3 * k; };
        REQUIRE_EQ(CT_type_lambda_generic_fromparams_3(f, 1, 1, 1),
                   "lambda: (int, int, int) -> int");
    }
}

auto lambda_std = [](int a) { return a * a; };
auto lambda_generic = [](int a) { return a * 2; };
auto lambda_composed = fplus::fwd::compose(lambda_std, lambda_generic);

TEST_CASE("lambda_generic_composition")
{
    {
        REQUIRE_EQ(CT_type_lambda_generic_fromparams_1(lambda_composed, 1),
                   "lambda: (int &&) -> int");
    }
    {
        auto local_lambda_std = [](auto a) { return a * a; };
        auto local_lambda_generic = [](auto a) { return a * 2; };
        auto local_lambda_composed = fplus::fwd::compose(local_lambda_std, local_lambda_generic);
        REQUIRE_EQ(CT_type_lambda_generic_fromparams_1(local_lambda_composed, 1),
                   "lambda: (int &&) -> int");
    }
}
