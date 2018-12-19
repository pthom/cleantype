// This file is part of ConsType : Consistent Type names for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once

#include <cleantype/cleantype.hpp>

namespace cleantype
{
    namespace internal
    {

        template <typename... Args, typename GenericLambda> std::string impl_lambda_generic(GenericLambda fn, bool clean_type)
        {
            #ifdef _MSC_VER
                auto ptr = &GenericLambda::operator() < Args... > ;
            #else
                auto ptr = &GenericLambda::template operator() < Args... > ;
            #endif
            auto as_mem_fn = std::mem_fn(ptr);

            // Do not group ptr and std::mem_fn() !
            // The version belows fails on fcc (compiler bug probably)
            // see https://stackoverflow.com/questions/53681778/generic-lambdas-mem-fn-with-gcc
            // auto as_mem_fn = std::mem_fn(
            //     & GenericLambda::template operator()<Args...> );

            std::string mem_fn_type = cleantype::internal::_impl_typeid_hana_no_holder<decltype(as_mem_fn)>();
            std::string final_type = cleantype::internal::_mem_fn_to_lambda_type(mem_fn_type, clean_type);
            return final_type;
        }


    } // namespace internal

    template <typename... Args, typename GenericLambda> std::string lambda_generic_clean(GenericLambda fn)
    {
        return internal::impl_lambda_generic<Args...>(fn, true);
    }

    template <typename... Args, typename GenericLambda> std::string lambda_generic_full(GenericLambda fn)
    {
        return internal::impl_lambda_generic<Args...>(fn, false);
    }


    //////////////////////////////
    // Start of public API
    //////////////////////////////

    template <typename... Args, typename GenericLambda> std::string lambda_generic_clean(GenericLambda fn);
    template <typename... Args, typename GenericLambda> std::string lambda_generic_full(GenericLambda fn);

} // namespace cleantype

// TN_type_lamda_generic_fromparams_
#define TN_type_lamda_generic_fromparams_1(fn, arg1) cleantype::lambda_generic_clean<decltype(arg1)>(fn)
#define TN_type_lamda_generic_fromparams_2(fn, arg1, arg2) cleantype::lambda_generic_clean<decltype(arg1), decltype(arg2)>(fn)
#define TN_type_lamda_generic_fromparams_3(fn, arg1, arg2, arg3) cleantype::lambda_generic_clean<decltype(arg1), decltype(arg2), decltype(arg3)>(fn)
#define TN_type_lamda_generic_fromparams_4(fn, arg1, arg2, arg3, arg4) cleantype::lambda_generic_clean<decltype(arg1), decltype(arg2), decltype(arg3), decltype(arg4)>(fn)
#define TN_type_lamda_generic_fromparams_5(fn, arg1, arg2, arg3, arg4, arg5) cleantype::lambda_generic_clean<decltype(arg1), decltype(arg2), decltype(arg3), decltype(arg4), decltype(arg5)>(fn)
