#pragma once
#include <type_name/details/type_name_full.hpp>
#include <type_name/details/type_name_clean.hpp>
#include <type_name/details/type_name_lambda.hpp>
#include <type_name/details/type_name_lambda_generic.hpp>
#include <type_name/details/type_name_function.hpp>


namespace type_name
{
    /////////////////////////
    // full types
    /////////////////////////

    // * `type_name::full<T>()` is a function that will return a string containing
    //    the full type of a variable.
    template <class T> std::string full();
    // * `type_name::show_details_full(T && v)` is a function that will return a string containing
    //    the full type of a variable, as well as its content
    template <class T> std::string show_details_full(T && v);

    // * `tn_type_name_full(var)` is a macro that will also return the full type,
    //    but, it is able to also correctly display rvalue reference types.

    // *  `tn_show_details_full(var)` is a macro that will return a string containing the name,
    //    type and content of a variable (in this case, the underlying type of 'var'
    //    has to have an 'ostream & operator<<')

    // * `tn_show_details_full_cont` is a version of tn_show_details_full for complex containers
    //    like "std::map". "cont" stands for "container".


    /////////////////////////
    // readable types
    /////////////////////////

    // * `type_name::clean<T>()` is a function that will return a string containing
    //    the readable type of a variable.
    template <class T> std::string clean();
    // * `type_name::show_details(T && v)` is a function that will return a string containing
    //    the readable type of a variable, as well as its content
    template <class T> std::string show_details(T && v);

    // * `tn_type_name_clean(var)` is a macro that will also return the full type,
    //    but, it is able to also correctly display rvalue reference types.

    // *  `tn_show_details(var)` is a macro that will return a string containing the name,
    //    type and content of a variable (in this case, the underlying type of 'var'
    //    has to have an 'ostream & operator<<')

    // * `tn_show_details_cont` (macro) is a version of tn_show_details for complex containers
    //    like "std::map". "cont" stands for "container".


    /////////////////////////
    // lambdas (non generic)
    /////////////////////////

    // * `type_name::lambda_clean<LambdaFn>(LambdaFn fn)` is a function that will return a string containing
    //    the readable signature of a lambda (non generic)
    template <typename LambdaFn> std::string lambda_clean(LambdaFn fn);
    // * `type_name::lambda_clean<LambdaFn>(LambdaFn fn)` is a function that will return a string containing
    //    the full signature of a lambda (non generic)
    template <typename LambdaFn> std::string lambda_clean(LambdaFn fn);

    // *  `tn_show_details_lambda(var)` is a macro that will return a string containing the
    //    readable signature of a lambda and its name

    // *  `tn_show_details_lambda_full(var)` is a macro that will return a string containing the
    //    full signature of a lambda and its name
}


// FIXME /  TODO : Add doc here !!! (And link)

/*
  tn_type_name_full
  tn_show_details_full
  tn_show_details_full_cont

  tn_type_name_clean(var)
  show_details(var)
  show_details_cont(var)

  lambda_full
  lambda_clean
  tn_show_details_lambda
  tn_show_details_lambda_full

  show_type_lambda_generic_fromparams_1
  show_type_lambda_generic_fromtypes_1
  show_details_lambda_generic_fromparams_1
  show_details_lambda_generic_fromtypes_1

  show_type_function_fromparams_1
*/