// This file is part of ConsType : Consistent Type names for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#include <constype/constype.hpp>
#include <constype/details/fp_polyfill/fp_polyfill.hpp>

#ifndef _MSC_VER


namespace constype
{
    namespace internal
    {
        auto wrap_function = [](auto &&f)
        {
            return [&](auto... args) {
                return f(args...);
            };
        };

        inline std::string lambda_to_fn(std::string const & s)
        {
            return fp::replace_tokens("lambda: ", "function: ", s);
        }
    } // namespace internal

} // namespace constype

#define show_type_function_fromparams_1(f, arg1) constype::internal::lambda_to_fn(\
  TN_type_lamda_generic_fromparams_1(constype::internal::wrap_function(f), arg1))
#define show_type_function_fromparams_2(f, arg1, arg2) constype::internal::lambda_to_fn(\
  TN_type_lamda_generic_fromparams_2(constype::internal::wrap_function(f), arg1, arg2))
#define show_type_function_fromparams_3(f, arg1, arg2, arg3) constype::internal::lambda_to_fn(\
  TN_type_lamda_generic_fromparams_3(constype::internal::wrap_function(f), arg1, arg2, arg3))
#define show_type_function_fromparams_4(f, arg1, arg2, arg3, arg4) constype::internal::lambda_to_fn(\
  TN_type_lamda_generic_fromparams_4(constype::internal::wrap_function(f), arg1, arg2, arg3, arg4))
#define show_type_function_fromparams_5(f, arg1, arg2, arg3, arg4, arg5) constype::internal::lambda_to_fn(\
  TN_type_lamda_generic_fromparams_5(constype::internal::wrap_function(f), arg1, arg2, arg3, arg4, arg5))

#define show_type_function_fromtypes_1(f, arg1) constype::internal::lambda_to_fn(\
  show_type_lambda_generic_fromtypes_1(constype::internal::wrap_function(f), arg1))
#define show_type_function_fromtypes_2(f, arg1, arg2) constype::internal::lambda_to_fn(\
  show_type_lambda_generic_fromtypes_2(constype::internal::wrap_function(f), arg1, arg2))
#define show_type_function_fromtypes_3(f, arg1, arg2, arg3) constype::internal::lambda_to_fn(\
  show_type_lambda_generic_fromtypes_3(constype::internal::wrap_function(f), arg1, arg2, arg3))
#define show_type_function_fromtypes_4(f, arg1, arg2, arg3, arg4) constype::internal::lambda_to_fn(\
  show_type_lambda_generic_fromtypes_4(constype::internal::wrap_function(f), arg1, arg2, arg3, arg4))
#define show_type_function_fromtypes_5(f, arg1, arg2, arg3, arg4, arg5) constype::internal::lambda_to_fn(\
  show_type_lambda_generic_fromtypes_5(constype::internal::wrap_function(f), arg1, arg2, arg3, arg4, arg5))

#endif // #ifndef _MSC_VER