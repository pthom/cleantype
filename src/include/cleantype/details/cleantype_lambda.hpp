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

        // template <typename LambdaFunction>
        // std::string type_lambda(LambdaFunction fn, bool clean_params)
        // {
        //     auto as_ptr = &LambdaFunction::operator();
        //     auto as_mem_fn = std::mem_fn(as_ptr);
        //     std::string mem_fn_type = cleantype::internal::_impl_typeid_hana<decltype(as_mem_fn)>();
        //     return _mem_fn_to_lambda_type(mem_fn_type, clean_params);
        // }

        template <typename First, typename... Rest, typename Lambda>
        std::string impl_lambda(Lambda fn, bool clean_type)
        {
            std::string mem_fn_type;
            {
                #ifdef _MSC_VER
                    auto as_ptr = &Lambda::operator() < First, Rest... > ;
                #else
                    auto as_ptr = &Lambda::template operator() < First, Rest... > ;
                #endif
                auto as_mem_fn = std::mem_fn(as_ptr);
                mem_fn_type = cleantype::internal::_impl_typeid_hana<decltype(as_mem_fn)>();
            }

            std::string final_type = cleantype::internal::_mem_fn_to_lambda_type(mem_fn_type, clean_type);
            return final_type;
        }

        template <typename Lambda> std::string impl_lambda(Lambda fn, bool clean_type)
        {
            std::string mem_fn_type;
            {
                auto as_ptr = &Lambda::operator(); // if you have an error here, your lambda is generic! Add template params for its input types!
                auto as_mem_fn = std::mem_fn(as_ptr);
                mem_fn_type = cleantype::internal::_impl_typeid_hana<decltype(as_mem_fn)>();
            }
            std::string final_type = cleantype::internal::_mem_fn_to_lambda_type(mem_fn_type, clean_type);
            return final_type;
        }


    } // namespace internal


    //////////////////////////////
    // Start of public API
    //////////////////////////////

    // * `cleantype::lambda_clean<typename... Args, typename Lambda>(Lambda fn)` is a function that will return a string containing
    //    the readable signature of a lambda
    //
    // In the case of a generic lambda, you will need to specify the type of the auto parameters:
    // Example:
    //    auto f = [](auto x, auto y) { return x + y; };
    //    std::cout << cleantype::lambda_clean<int, char>(f) << std::endl;
    //     ==>   lambda: (int, char) -> int
    template <typename... Args, typename Lambda> std::string lambda_clean(Lambda fn)
    {
        return internal::impl_lambda<Args...>(fn, true);
    }

    // * `cleantype::lambda_clean<typename... Args, typename Lambda>(Lambda fn)` is a function that will return a string containing
    //    the full signature of a lambda
    // In the case of a generic lambda, you will need to specify the type of the auto parameters
    // (see lambda_clean doc for an example)
    template <typename... Args, typename Lambda> std::string lambda_full(Lambda fn)
    {
        return internal::impl_lambda<Args...>(fn, false);
    }

} // namespace cleantype

#define CT_show_details_lambda(f) std::string("[") + cleantype::lambda_clean(f) + "] " + #f
#define CT_show_details_lambda_full(f) std::string("[") + cleantype::lambda_full(f) + "] " + #f

// CT_type_lambda_generic_fromparams_
#define CT_type_lambda_generic_fromparams_1(fn, arg1) cleantype::lambda_clean<decltype(arg1)>(fn)
#define CT_type_lambda_generic_fromparams_2(fn, arg1, arg2) cleantype::lambda_clean<decltype(arg1), decltype(arg2)>(fn)
#define CT_type_lambda_generic_fromparams_3(fn, arg1, arg2, arg3) cleantype::lambda_clean<decltype(arg1), decltype(arg2), decltype(arg3)>(fn)
#define CT_type_lambda_generic_fromparams_4(fn, arg1, arg2, arg3, arg4) cleantype::lambda_clean<decltype(arg1), decltype(arg2), decltype(arg3), decltype(arg4)>(fn)
#define CT_type_lambda_generic_fromparams_5(fn, arg1, arg2, arg3, arg4, arg5) cleantype::lambda_clean<decltype(arg1), decltype(arg2), decltype(arg3), decltype(arg4), decltype(arg5)>(fn)
