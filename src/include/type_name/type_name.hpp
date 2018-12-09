#pragma once
#include <type_name/details/type_name_full.hpp>
#include <type_name/details/type_name_clean.hpp>
#include <type_name/details/type_name_lambda.hpp>
#include <type_name/details/type_name_lambda_generic.hpp>
#include <type_name/details/type_name_function.hpp>


namespace type_name
{

    // * `type_name::full(var)` is a function that will return a string containing
    //    the full type of a variable.
    template <class T> std::string full();

    // * `m_type_name_full(var)` is a macro that will also return the full type,
    //    but, it is able to also correctly display rvalue reference types.

    // *  `m_show_details_full(var)` will return a string containing the name,
    //    type and content of a variable (in this case, the underlying type of 'var'
    //    has to have an 'ostream & operator<<')

    // * `m_show_details_full_cont` is a version of m_show_details_full for complex containers
    //    like "std::map". "cont" stands for "container".

}


// FIXME /  TODO : Add doc here !!! (And link)

/*
  m_type_name_full
  m_show_details_full
  m_show_details_full_cont

  type_name_clean(var)
  show_details(var)
  show_details_cont(var)

  type_lambda_full
  type_lambda_clean
  show_details_lambda
  show_details_lambda_full

  show_type_lambda_generic_fromparams_1
  show_type_lambda_generic_fromtypes_1
  show_details_lambda_generic_fromparams_1
  show_details_lambda_generic_fromtypes_1

  show_type_function_fromparams_1
*/