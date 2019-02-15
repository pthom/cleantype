// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once
#include <cleantype/cleantype_configuration.hpp>
#include <cleantype/details/cleantype_clean.hpp>
#include <cleantype/details/cleantype_eastconst.hpp>
#include <cleantype/details/cleantype_full.hpp>
#include <cleantype/details/cleantype_invoke_result.hpp>
#include <cleantype/details/cleantype_lambda.hpp>

namespace cleantype
{
    /////////////////////////
    // readable types
    /////////////////////////
    // * `cleantype::clean<T...>()` is a function that will return a string containing
    //    a readable type, for a given type or pack of types
    //    Use it with `cleantype::clean<decltype(var)>()`
    //    Note:
    //      * It will add a reference by default so that
    //         int v = 5; cleantype::clean(v) will return "int&"
    //         => use the macro CT_cleantype_clean() if you want to avoid this

    // * `cleantype::clean<T...>(t...)` is a an easier version, using an instance of the type.
    //    Notes:
    //      * It will add a reference by default so that
    //         int v = 5; cleantype::clean(v) will return "int&"
    //         => use the macro CT_cleantype_clean() if you want to avoid this
    //      * It is not able to output correctly r-value references
    //          For this, use `cleantype::clean<decltype(var)>()`

    // * `cleantype::show_details(T && v)` is a function that will return a string containing
    //    the readable type of a variable, as well as its content

    // * `CT_cleantype_clean(var)` is a macro that will also return the full type,
    //    but, it is able to also correctly display rvalue reference types.

    // *  `CT_show_details(var)` is a macro that will return a string containing the name,
    //    type and content of a variable (in this case, the underlying type of 'var'
    //    has to have an 'ostream & operator<<')

    // * `CT_show_details_cont` (macro) is a version of CT_show_details for complex containers
    //    like "std::map". "cont" stands for "container".

    // * `cleantype::clean_typestring(type_names)` will clean a given (list of) type(s) given as a string
    // * `cleantype::indent_type_tree(type_names)` will present an indented view of a given (list of) type(s).

    /////////////////////////
    // full types
    /////////////////////////
    // * `cleantype::full<T...>()` is a function that will return a string containing
    //    the full type. It also works with packs of types.
    //    Use it with `cleantype::full<decltype(var)>()`
    //    Note:
    //      * It will add a reference by default so that
    //         int v = 5; cleantype::full(v) will return "int&"
    //         => use the macro CT_cleantype_full() if you want to avoid this

    // * `cleantype::full<T...>(t...)` is a an easier version, using an instance of the type.
    //    Notes:
    //      * It will add a reference by default so that
    //         int v = 5; cleantype::full(v) will return "int&"
    //         => use the macro CT_cleantype_full() if you want to avoid this
    //      * It is not able to output correctly r-value references
    //          For this, use `cleantype::full<decltype(var)>()`

    // * `cleantype::show_details_full(T && v)` is a function that will return a string containing
    //    the full type of a variable, as well as its content

    // * `CT_cleantype_full(var)` is a macro that will also return the full type,
    //    but, it is able to also correctly display rvalue reference types.

    // *  `CT_show_details_full(var)` is a macro that will return a string containing the name,
    //    type and content of a variable (in this case, the underlying type of 'var'
    //    has to have an 'ostream & operator<<')

    // * `CT_show_details_full_cont` is a version of CT_show_details_full for complex containers
    //    like "std::map". "cont" stands for "container".


    /////////////////////////////////////////
    // decipher type in your compiler output
    /////////////////////////////////////////
    // `ct_compiler_decipher` simplifies the template noise in your compiler output :
    // just "|" (pipe) your build tool to it.
    // Compile it with the following command:
    // $CC -Isrc/include --std=c++14 src/tools/ct_compiler_decipher/ct_compiler_decipher.cpp -o ct_compiler_decipher

    /////////////////////////////////////////
    // identify types in the compiler output
    /////////////////////////////////////////
    // * `CT_compiler_log_var_type(var)` is a macro that will create
    //   an intentional compile error, in which you will be able to read
    //   the variable type.
    // * `CT_compiler_log_type(var)` is a macro that will create
    //   an intentional compile error, in which you will be able to read
    //   the type definition
    // You can use these macros in combinaation with `ct_compiler_decipher`
    // (pipe your build output to `ct_compiler_decipher` in order to get
    //  readable types)

    /////////////////////////////////////////
    // Compile time constexpr type names
    /////////////////////////////////////////
    // cleantype::full_compiletime<T>() will give you the full name of a type in the form of a Boost.Hana string.
    // Note: For this, you need to manually include "cleantype/cleantype_compiler_typename.hpp",
    // and to have boost in your include path.

    ///////////////////////////////////////////////////////////
    // Identify the auto return type of functions and functors
    ///////////////////////////////////////////////////////////
    // * cleantype::invoke_result<F, Args...>::type and cleantype::invoke_result_t<F, Args...>
    //   will contain the type of any function / lambda / functor. This is a type, not a string.
    // * CT_invoke_result_fn(F, Args...) and CT_invoke_result_fn_template(F, Args...)
    //   are macros that makes it easy to get the return type of a function whose return type
    //   is marked as "auto" (the second version is for template functions)
    // * CT_type_fn, CT_type_fn_template, CT_type_fn_full and CT_type_fn_template_full
    //   are macros that return a string containing the return type of a function
    //   whose return type is marked as "auto"

    /////////////////////////
    // lambdas
    /////////////////////////
    // * `cleantype::lambda_clean<typename... Args, typename Lambda>(Lambda fn)` is a function that
    // will return a string containing
    //    the readable signature of a lambda
    //
    // In the case of a generic lambda, you will need to specify the type of the auto parameters:
    // Example:
    //    auto f = [](auto x, auto y) { return x + y; };
    //    std::cout << cleantype::lambda_clean<int, char>(f) << std::endl;
    //     ==>   lambda: (int, char) -> int

    // * `cleantype::lambda_clean<typename... Args, typename Lambda>(Lambda fn)` is a function that
    // will return a string containing
    //    the full signature of a lambda

    // * `cleantype::lambda<typename... Args, typename Lambda>(Lambda fn, bool flag_clean)` is a
    // function that will return
    //    a string containing the signature of a lambda.
    //    flag_clean controls wether the signature is cleaned or not.
    //
    // In the case of a generic lambda, you will need to specify the type of the auto parameters:
    // Example:
    //    auto f = [](auto x, auto y) { return x + y; };
    //    std::cout << cleantype::lambda<int, char>(f, true) << std::endl;
    //     ==>   lambda: (int, char) -> int

    // *  `CT_show_details_lambda(var)` is a macro that will return a string containing the
    //    readable signature of a lambda and its name

    // *  `CT_show_details_lambda_full(var)` is a macro that will return a string containing the
    //    full signature of a lambda and its name

    // *  `CT_type_lambda_generic_fromparams_XXX(lambda, arg1, arg2, ...)` is a macro that will
    // return a string containing the
    //     signature of a generic lambda where you do not specify the args type, instead you give
    //     example of these types. (XXX is the number of params of the lambda, and can vary from 1
    //     to 5).


    /////////////////////////////////////////
    // Settings - configure replacements and indentation
    /////////////////////////////////////////
    // In order to configure the behavior of cleantype:
    // Duplicate the file .cleantype.json at the root of the cleantype repository,
    // and place it wherever in the hierarchy of the parent directories of your application.
    // Edit this file as a normal json file

}  // namespace cleantype
