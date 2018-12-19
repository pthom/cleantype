// This file is part of ConsType : Consistent Type names for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once
#include <cleantype/cleantype_configuration.hpp>
#include <cleantype/details/cleantype_full.hpp>
#include <cleantype/details/cleantype_clean.hpp>
#include <cleantype/details/cleantype_lambda.hpp>
#include <cleantype/details/cleantype_lambda_generic.hpp>
#include <cleantype/details/cleantype_function.hpp>


namespace cleantype
{
    /////////////////////////
    // full types
    /////////////////////////

    // * `cleantype::full<T...>()` is a function that will return a string containing
    //    the full type. It also works with packs of types.
    //    Use it with `cleantype::full<decltype(var)>()`
    //    Note:
    //      * It will add a reference by default so that
    //         int v = 5; cleantype::full(v) will return "int&"
    //         => use the macro TN_cleantype_full() if you want to avoid this

    // * `cleantype::full<T...>(t...)` is a an easier version, using an instance of the type.
    //    Notes:
    //      * It will add a reference by default so that
    //         int v = 5; cleantype::full(v) will return "int&"
    //         => use the macro TN_cleantype_full() if you want to avoid this
    //      * It is not able to output correctly r-value references
    //          For this, use `cleantype::full<decltype(var)>()`

    // * `cleantype::show_details_full(T && v)` is a function that will return a string containing
    //    the full type of a variable, as well as its content

    // * `TN_cleantype_full(var)` is a macro that will also return the full type,
    //    but, it is able to also correctly display rvalue reference types.

    // *  `TN_show_details_full(var)` is a macro that will return a string containing the name,
    //    type and content of a variable (in this case, the underlying type of 'var'
    //    has to have an 'ostream & operator<<')

    // * `TN_show_details_full_cont` is a version of TN_show_details_full for complex containers
    //    like "std::map". "cont" stands for "container".


    /////////////////////////
    // readable types
    /////////////////////////

    // * `cleantype::clean<T...>()` is a function that will return a string containing
    //    a readable type, for a given type or pack of types
    //    Use it with `cleantype::clean<decltype(var)>()`
    //    Note:
    //      * It will add a reference by default so that
    //         int v = 5; cleantype::clean(v) will return "int&"
    //         => use the macro TN_cleantype_clean() if you want to avoid this

    // * `cleantype::clean<T...>(t...)` is a an easier version, using an instance of the type.
    //    Notes:
    //      * It will add a reference by default so that
    //         int v = 5; cleantype::clean(v) will return "int&"
    //         => use the macro TN_cleantype_clean() if you want to avoid this
    //      * It is not able to output correctly r-value references
    //          For this, use `cleantype::clean<decltype(var)>()`

    // * `cleantype::show_details(T && v)` is a function that will return a string containing
    //    the readable type of a variable, as well as its content

    // * `TN_cleantype_clean(var)` is a macro that will also return the full type,
    //    but, it is able to also correctly display rvalue reference types.

    // *  `TN_show_details(var)` is a macro that will return a string containing the name,
    //    type and content of a variable (in this case, the underlying type of 'var'
    //    has to have an 'ostream & operator<<')

    // * `TN_show_details_cont` (macro) is a version of TN_show_details for complex containers
    //    like "std::map". "cont" stands for "container".


    /////////////////////////
    // lambdas (non generic)
    /////////////////////////

    // * `cleantype::lambda_clean<LambdaFn>(LambdaFn fn)` is a function that will return a string containing
    //    the readable signature of a lambda (non generic)
    template <typename LambdaFn> std::string lambda_clean(LambdaFn fn);
    // * `cleantype::lambda_clean<LambdaFn>(LambdaFn fn)` is a function that will return a string containing
    //    the full signature of a lambda (non generic)
    template <typename LambdaFn> std::string lambda_clean(LambdaFn fn);

    // *  `TN_show_details_lambda(var)` is a macro that will return a string containing the
    //    readable signature of a lambda and its name

    // *  `TN_show_details_lambda_full(var)` is a macro that will return a string containing the
    //    full signature of a lambda and its name

    /////////////////////////
    // lambdas ( generic)
    /////////////////////////
    // * `cleantype::lambda_generic_clean<Args...>(LambdaFn fn)` is a function that will return a string containing
    //    the readable signature of a generic lambda.
    // You will need to specify the type of the auto parameters
    // Example:
    //    auto f = [](auto x, auto y) { return x + y; };
    //    std::cout << cleantype::lambda_generic_clean<int, char>(f) << std::endl;
    //     ==>   lambda: (int, char) -> int
    template <typename... Args, typename GenericLambda> std::string lambda_generic_clean(GenericLambda fn);

    // * `cleantype::lambda_generic_full<Args...>(LambdaFn fn)` is a function that will return a string containing
    //    the full signature of a generic lambda.
    template <typename... Args, typename GenericLambda> std::string lambda_generic_full(GenericLambda fn);

    // *  `TN_type_lamda_generic_fromparams_XXX(lambda, arg1, arg2, ...)` is a macro that will return a string containing the
    //     signature of a generic lambda where you do not specify the args type, instead you give example of these types.
    //     (XXX is the number of params of the lambda, and can vary from 1 to 5).

}


// FIXME /  TODO : Add doc here !!! (And link)
