#include "doctest.h"
#include <type_name/type_name.hpp>
#include <fplus/fplus.hpp>
#include <functional>
#include <map>

#if !defined(__clang__) && ( defined(__GNUC__) || defined(__GNUG__) )
    #define IS_GCC_COMPILER
#endif


#define LOG(str) std::cout << str << std::endl

template <typename... Args, typename GenericLambda>
auto lambda_to_mem_fn2(GenericLambda generic_lambda)
{
    #ifdef _MSC_VER
        auto ptr = &GenericLambda::operator() < Args... > ;
    #else
        auto ptr = &GenericLambda::template operator() < Args... > ;
    #endif
    auto as_mem_fn = std::mem_fn(ptr);

    // The version belows fails on fcc (compiler bug probably)
    // see https://stackoverflow.com/questions/53681778/generic-lambdas-mem-fn-with-gcc
    // auto as_mem_fn = std::mem_fn(
    //     & GenericLambda::template operator()<Args...> );

    return as_mem_fn;
}

// namespace type_name
// {
//     // * `type_name::full<T>()` is a function that will return a string containing
//     //    the full type of a variable.
//     template <typename... Args> std::string full()
//     {
//         return "";
//     }

// }


template <typename... Args, typename GenericLambda> std::string lambda_generic_clean2(GenericLambda fn)
{
    #ifdef _MSC_VER
        auto ptr = &GenericLambda::operator() < Args... > ;
    #else
        auto ptr = &GenericLambda::template operator() < Args... > ;
    #endif
    auto as_mem_fn = std::mem_fn(ptr);
    std::string mem_fn_type = type_name::full < decltype(as_mem_fn) >();
    bool clean_type = true;
    std::string final_type = type_name::internal::_mem_fn_to_lambda_type(mem_fn_type, clean_type);
    return final_type;
}

TEST_CASE("tuc")
{
    auto f = [](auto i) { return 2 * i; };
    //auto mem_fn = lambda_to_mem_fn2<int>(f);
    //std::cout << type_name::clean< decltype(mem_fn) >();

    std::cout << "--->" << lambda_generic_clean2<int>(f) << "<---\n";
}

// template <typename LambdaFn, typename... Args> std::string lambda_generic_clean(LambdaFn fn)
// template <typename LambdaFunction, typename... Args> auto lambda_to_mem_fn()

// auto make_lambda_generic_clean = []( auto f ) -> std::string {
//     return type_name::lambda_generic_clean<decltype(f)>();
// };

// template<class T1, class T2>
// std::pair<T1, T2> my_make_pair(T1 t1, T2 t2) {
//     return std::pair<T1, T2>(t1, t2);
// }

    template <typename LambdaFn, typename... Args> auto make_holder(LambdaFn fn)
    {
        return type_name::internal::lambda_generic_type_holder<LambdaFn, Args...>();
    }



TEST_CASE("lambda_generic_clean")
{
    {
        auto f = [](auto i) { return 2 * i; };
        std::string s = type_name::lambda_generic_clean< decltype(f), int >(f);
        LOG(s);


        //type_name::lambda_generic_clean2(f);

        //auto mem_fn = type_name::internal::lambda_to_mem_fn< decltype(f), int >();
        //auto f2 = make_lambda_generic_clean::operator()<int>(f);

        //auto holder = make_holder< decltype(f), int >(f);
    }
}


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
