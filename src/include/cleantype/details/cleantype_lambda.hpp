// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once
#include <cleantype/details/cleantype_clean.hpp>
#include <cleantype/cleantype_configuration.hpp>
#include <cleantype/details/cleantype_lambda_parse.hpp>

namespace cleantype
{
    namespace internal
    {

        template <typename LambdaFunction>
        std::string type_lambda(LambdaFunction fn, bool clean_params)
        {
            auto as_ptr = &LambdaFunction::operator();
            auto as_mem_fn = std::mem_fn(as_ptr);
            std::string mem_fn_type = cleantype::internal::_impl_typeid_hana<decltype(as_mem_fn)>();
            return _mem_fn_to_lambda_type(mem_fn_type, clean_params);
        }

    } // namespace internal

    template <typename LambdaFunction>
    std::string lambda_full(LambdaFunction fn)
    {
        return internal::type_lambda(fn, false);
    }

    template <typename LambdaFunction>
    std::string lambda_clean(LambdaFunction fn)
    {
        return internal::type_lambda(fn, true);
    }


    //////////////////////////////
    // Start of public API
    //////////////////////////////

    // * `cleantype::lambda_clean<LambdaFn>(LambdaFn fn)` is a function that will return a string containing
    //    the readable signature of a lambda (non generic)
    template <typename LambdaFn> std::string lambda_clean(LambdaFn fn);
    // * `cleantype::lambda_clean<LambdaFn>(LambdaFn fn)` is a function that will return a string containing
    //    the full signature of a lambda (non generic)
    template <typename LambdaFn> std::string lambda_clean(LambdaFn fn);

} // namespace cleantype

#define CT_show_details_lambda(f) std::string("[") + cleantype::lambda_clean(f) + "] " + #f
#define CT_show_details_lambda_full(f) std::string("[") + cleantype::lambda_full(f) + "] " + #f
