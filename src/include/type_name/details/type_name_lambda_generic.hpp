#pragma once

#include <type_name/type_name.hpp>
#include <type_name/details/apply_args.hpp>

namespace type_name
{
    namespace internal
    {

        template <typename GenericLambda, typename... Args>
        auto lambda_to_mem_fn()
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


        template <typename GenericLambda, typename... Args>
        struct lambda_generic_type_holder {

            std::string type_name;

            lambda_generic_type_holder(bool clean_type = true)
            {
                auto as_mem_fn = lambda_to_mem_fn<GenericLambda, Args...>();

                std::string mem_fn_type = tn_type_name_full(as_mem_fn);
                type_name = type_name::internal::_mem_fn_to_lambda_type(mem_fn_type, clean_type);
            }
        };


    } // namespace internal

    template <typename GenericLambda, typename... Args> std::string lambda_generic_clean(GenericLambda fn)
    {
        internal::lambda_generic_type_holder<GenericLambda, Args...> holder;
        return holder.type_name;
    }


    //////////////////////////////
    // Start of public API
    //////////////////////////////

} // namespace type_name


#define show_type_lambda_generic_fromparams_1(fn, arg1) type_name::internal::lambda_generic_type_holder<decltype(fn), decltype(arg1)>().type_name
#define show_type_lambda_generic_fromparams_2(fn, arg1, arg2) type_name::internal::lambda_generic_type_holder<decltype(fn), decltype(arg1), decltype(arg2)>().type_name
#define show_type_lambda_generic_fromparams_3(fn, arg1, arg2, arg3) type_name::internal::lambda_generic_type_holder<decltype(fn), decltype(arg1), decltype(arg2), decltype(arg3)>().type_name
#define show_type_lambda_generic_fromparams_4(fn, arg1, arg2, arg3, arg4) type_name::internal::lambda_generic_type_holder<decltype(fn), decltype(arg1), decltype(arg2), decltype(arg3), decltype(arg4)>().type_name
#define show_type_lambda_generic_fromparams_5(fn, arg1, arg2, arg3, arg4, arg5) type_name::internal::lambda_generic_type_holder<decltype(fn), decltype(arg1), decltype(arg2), decltype(arg3), decltype(arg4), decltype(arg5)>().type_name

#define show_details_lambda_generic_fromparams_1(fn, arg1) std::string("[") + show_type_lambda_generic_fromparams_1(fn, arg1) + "] " + #fn
#define show_details_lambda_generic_fromparams_2(fn, arg1, arg2) std::string("[") + show_type_lambda_generic_fromparams_2(fn, arg1, arg2) + "] " + #fn
#define show_details_lambda_generic_fromparams_3(fn, arg1, arg2, arg3) std::string("[") + show_type_lambda_generic_fromparams_3(fn, arg1, arg2, arg3) + "] " + #fn
#define show_details_lambda_generic_fromparams_4(fn, arg1, arg2, arg3, arg4) std::string("[") + show_type_lambda_generic_fromparams_4(fn, arg1, arg2, arg3, arg4) + "] " + #fn
#define show_details_lambda_generic_fromparams_5(fn, arg1, arg2, arg3, arg4, arg5) std::string("[") + show_type_lambda_generic_fromparams_5(fn, arg1, arg2, arg3, arg4, arg5) + "] " + #fn



#define show_type_lambda_generic_fromtypes_1(fn, arg1) type_name::internal::lambda_generic_type_holder<decltype(fn), arg1>().type_name
#define show_type_lambda_generic_fromtypes_2(fn, arg1, arg2) type_name::internal::lambda_generic_type_holder<decltype(fn), arg1, arg2>().type_name
#define show_type_lambda_generic_fromtypes_3(fn, arg1, arg2, arg3) type_name::internal::lambda_generic_type_holder<decltype(fn), arg1, arg2, arg3>().type_name
#define show_type_lambda_generic_fromtypes_4(fn, arg1, arg2, arg3, arg4) type_name::internal::lambda_generic_type_holder<decltype(fn), arg1, arg2, arg3, arg4>().type_name
#define show_type_lambda_generic_fromtypes_5(fn, arg1, arg2, arg3, arg4, arg5) type_name::internal::lambda_generic_type_holder<decltype(fn), arg1, arg2, arg3, arg4, arg5>().type_name

#define show_details_lambda_generic_fromtypes_1(fn, arg1) std::string("[") + show_type_lambda_generic_fromtypes_1(fn, arg1) + "] " + #fn
#define show_details_lambda_generic_fromtypes_2(fn, arg1, arg2) std::string("[") + show_type_lambda_generic_fromtypes_2(fn, arg1, arg2) + "] " + #fn
#define show_details_lambda_generic_fromtypes_3(fn, arg1, arg2, arg3) std::string("[") + show_type_lambda_generic_fromtypes_3(fn, arg1, arg2, arg3) + "] " + #fn
#define show_details_lambda_generic_fromtypes_4(fn, arg1, arg2, arg3, arg4) std::string("[") + show_type_lambda_generic_fromtypes_4(fn, arg1, arg2, arg3, arg4) + "] " + #fn
#define show_details_lambda_generic_fromtypes_5(fn, arg1, arg2, arg3, arg4, arg5) std::string("[") + show_type_lambda_generic_fromtypes_5(fn, arg1, arg2, arg3, arg4, arg5) + "] " + #fn
