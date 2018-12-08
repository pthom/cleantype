#include "doctest.h"
#include <type_name/type_name.hpp>
#include <fplus/fplus.hpp>
#include <functional>
#include <map>

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


auto my_square22 = [](int a) { return a * a; };
auto my_double22 = [](int a) { return a * 2; };
auto f22 = fplus::fwd::compose(my_square22, my_double22);


TEST_CASE("log_type_lambda_clean___compose")
{

    {
        auto my_square = [](int a) { return a * a; };
        auto my_double = [](auto a) { return a * 2; };
        auto f = fplus::fwd::compose(my_square, my_double);

        auto double_auto = [](auto x) { return 2. * x; };

        //using namespace type_name;

        // debug variadic macros : https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html
        // LOG(v2_type_lambda_generic(f, int));

        auto f4 = [](auto x, auto y) {
            return x + y;
        };
        // LOG(show_details_lambda(my_square));
        // LOG(show_details_lambda_generic_fromparams_1(my_double, 1));
        // LOG(show_details_lambda_generic_fromparams_1(f22, 4));
        // LOG(show_details_lambda_generic_fromparams_1(f, 4));      // (_DOCTEST_ANON_FUNC_2()::{lambda(auto:1, auto:2)#4}::*)(int, double)
        // LOG(show_details_lambda_generic_fromparams_2(f4, 3, 4.)); // (int, double) -> double] f4
        // std::cout << "Wowwwww";
    }
}

int my_function(int a, int b)
{
    return a + b;
}

auto wrap_function = [](auto &&f) {
    return [&](auto... args) {
        return f(args...);
    };
};

TEST_CASE("log_wrap_function")
{
    auto f_log_wrap_function = wrap_function(my_function);
    // Does not work under gcc...
    //LOG(show_details_lambda_generic_fromparams_2(f_log_wrap_function, 2, 3));
}

// TODO :
// - finish variadic lambdas
// - type of polymorphic lambdas
// - type of functions (wrap in std::function ?)
