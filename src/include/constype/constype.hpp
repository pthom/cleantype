#pragma once
#include <constype/constype_configuration.hpp>
#include <constype/details/constype_full.hpp>
#include <constype/details/constype_clean.hpp>
#include <constype/details/constype_lambda.hpp>
#include <constype/details/constype_lambda_generic.hpp>
#include <constype/details/constype_function.hpp>


namespace constype
{
    /////////////////////////
    // full types
    /////////////////////////

    // * `constype::full<T...>()` is a function that will return a string containing
    //    the full type. It also works with packs of types.
    //    Use it with `constype::full<decltype(var)>()`
    //    Note:
    //      * It will add a reference by default so that
    //         int v = 5; constype::full(v) will return "int&"
    //         => use the macro TN_constype_full() if you want to avoid this

    // * `constype::full<T...>(t...)` is a an easier version, using an instance of the type.
    //    Notes:
    //      * It will add a reference by default so that
    //         int v = 5; constype::full(v) will return "int&"
    //         => use the macro TN_constype_full() if you want to avoid this
    //      * It is not able to output correctly r-value references
    //          For this, use `constype::full<decltype(var)>()`

    // * `constype::show_details_full(T && v)` is a function that will return a string containing
    //    the full type of a variable, as well as its content

    // * `TN_constype_full(var)` is a macro that will also return the full type,
    //    but, it is able to also correctly display rvalue reference types.

    // *  `TN_show_details_full(var)` is a macro that will return a string containing the name,
    //    type and content of a variable (in this case, the underlying type of 'var'
    //    has to have an 'ostream & operator<<')

    // * `TN_show_details_full_cont` is a version of TN_show_details_full for complex containers
    //    like "std::map". "cont" stands for "container".


    /////////////////////////
    // readable types
    /////////////////////////

    // * `constype::clean<T...>()` is a function that will return a string containing
    //    a readable type, for a given type or pack of types
    //    Use it with `constype::clean<decltype(var)>()`
    //    Note:
    //      * It will add a reference by default so that
    //         int v = 5; constype::clean(v) will return "int&"
    //         => use the macro TN_constype_clean() if you want to avoid this

    // * `constype::clean<T...>(t...)` is a an easier version, using an instance of the type.
    //    Notes:
    //      * It will add a reference by default so that
    //         int v = 5; constype::clean(v) will return "int&"
    //         => use the macro TN_constype_clean() if you want to avoid this
    //      * It is not able to output correctly r-value references
    //          For this, use `constype::clean<decltype(var)>()`

    // * `constype::show_details(T && v)` is a function that will return a string containing
    //    the readable type of a variable, as well as its content

    // * `TN_constype_clean(var)` is a macro that will also return the full type,
    //    but, it is able to also correctly display rvalue reference types.

    // *  `TN_show_details(var)` is a macro that will return a string containing the name,
    //    type and content of a variable (in this case, the underlying type of 'var'
    //    has to have an 'ostream & operator<<')

    // * `TN_show_details_cont` (macro) is a version of TN_show_details for complex containers
    //    like "std::map". "cont" stands for "container".


    /////////////////////////
    // lambdas (non generic)
    /////////////////////////

    // * `constype::lambda_clean<LambdaFn>(LambdaFn fn)` is a function that will return a string containing
    //    the readable signature of a lambda (non generic)
    template <typename LambdaFn> std::string lambda_clean(LambdaFn fn);
    // * `constype::lambda_clean<LambdaFn>(LambdaFn fn)` is a function that will return a string containing
    //    the full signature of a lambda (non generic)
    template <typename LambdaFn> std::string lambda_clean(LambdaFn fn);

    // *  `TN_show_details_lambda(var)` is a macro that will return a string containing the
    //    readable signature of a lambda and its name

    // *  `TN_show_details_lambda_full(var)` is a macro that will return a string containing the
    //    full signature of a lambda and its name

    /////////////////////////
    // lambdas ( generic)
    /////////////////////////
    // * `constype::lambda_generic_clean<Args...>(LambdaFn fn)` is a function that will return a string containing
    //    the readable signature of a generic lambda.
    // You will need to specify the type of the auto parameters
    // Example:
    //    auto f = [](auto x, auto y) { return x + y; };
    //    std::cout << constype::lambda_generic_clean<int, char>(f) << std::endl;
    //     ==>   lambda: (int, char) -> int
    template <typename... Args, typename GenericLambda> std::string lambda_generic_clean(GenericLambda fn);

    // * `constype::lambda_generic_full<Args...>(LambdaFn fn)` is a function that will return a string containing
    //    the full signature of a generic lambda.
    template <typename... Args, typename GenericLambda> std::string lambda_generic_full(GenericLambda fn);

    // *  `TN_type_lamda_generic_fromparams_XXX(lambda, arg1, arg2, ...)` is a macro that will return a string containing the
    //     signature of a generic lambda where you do not specify the args type, instead you give example of these types.
    //     (XXX is the number of params of the lambda, and can vary from 1 to 5).

}


// FIXME /  TODO : Add doc here !!! (And link)
