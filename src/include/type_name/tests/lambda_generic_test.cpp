#include "doctest.h"
#include <type_name/type_name.hpp>
#include <fplus/fplus.hpp>
#include <functional>
#include <map>

#if !defined(__clang__) && ( defined(__GNUC__) || defined(__GNUG__) )
    #define IS_GCC_COMPILER
#endif


#define LOG(str) std::cout << str << std::endl



TEST_CASE("lambda_generic_1_to_5_params")
{
    {
        auto f = [](auto i) { return 2 * i; };
        REQUIRE_EQ(
            show_type_lambda_generic_fromparams_1(f, 1),
            "lambda: (int) -> int");
        REQUIRE_EQ(
            show_type_lambda_generic_fromtypes_1(f, int),
            "lambda: (int) -> int");
        REQUIRE_EQ(
            show_details_lambda_generic_fromparams_1(f, 1),
            "[lambda: (int) -> int] f");
        REQUIRE_EQ(
            show_details_lambda_generic_fromtypes_1(f, int),
            "[lambda: (int) -> int] f");
    }
    {
        auto f = [](auto i, auto j) { return 2 * i + j; };
        REQUIRE_EQ(
            show_type_lambda_generic_fromparams_2(f, 1, 1),
            "lambda: (int, int) -> int");
        REQUIRE_EQ(
            show_type_lambda_generic_fromtypes_2(f, int, int),
            "lambda: (int, int) -> int");
        REQUIRE_EQ(
            show_details_lambda_generic_fromparams_2(f, 1, 1),
            "[lambda: (int, int) -> int] f");
        REQUIRE_EQ(
            show_details_lambda_generic_fromtypes_2(f, int, int),
            "[lambda: (int, int) -> int] f");
    }
    {
        auto f = [](auto i, auto j, auto k) { return 2 * i + j + 3 * k; };
        REQUIRE_EQ(
            show_type_lambda_generic_fromparams_3(f, 1, 1, 1),
            "lambda: (int, int, int) -> int");
        REQUIRE_EQ(
            show_type_lambda_generic_fromtypes_3(f, int, int, int),
            "lambda: (int, int, int) -> int");
        REQUIRE_EQ(
            show_details_lambda_generic_fromparams_3(f, 1, 1, 1),
            "[lambda: (int, int, int) -> int] f");
        REQUIRE_EQ(
            show_details_lambda_generic_fromtypes_3(f, int, int, int),
            "[lambda: (int, int, int) -> int] f");
    }
}


auto lambda_std = [](int a) { return a * a; };
auto lambda_generic = [](int a) { return a * 2; };
auto lambda_composed = fplus::fwd::compose(lambda_std, lambda_generic);


#ifndef IS_GCC_COMPILER
TEST_CASE("lambda_generic_composition")
{

    {
        REQUIRE_EQ(
            show_type_lambda_generic_fromparams_1(lambda_composed, 1),
            "lambda: (int &&) -> int"
        );
    }
    {
        auto local_lambda_std = [](int a) { return a * a; };
        auto local_lambda_generic = [](int a) { return a * 2; };
        auto local_lambda_composed = fplus::fwd::compose(local_lambda_std, local_lambda_generic);
        REQUIRE_EQ(
            show_type_lambda_generic_fromparams_1(local_lambda_composed, 1),
            "lambda: (int &&) -> int"
        );
    }
}
#endif
