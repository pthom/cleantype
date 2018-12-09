#include <type_name/type_name.hpp>
#include <type_name/details/fp_polyfill/fp_polyfill.hpp>

#ifndef _MSC_VER


namespace type_name
{
    namespace internal
    {
        auto wrap_function = [](auto &&f)
        {
            return [&](auto... args) {
                return f(args...);
            };
        };

        inline std::string lambda_to_fn(const std::string & s)
        {
            return fp::replace_tokens("lambda: ", "function: ", s);
        }
    } // namespace internal

} // namespace type_name

#define show_type_function_fromparams_1(f, arg1) type_name::internal::lambda_to_fn(\
  tn_type_lamda_generic_fromparams_1(type_name::internal::wrap_function(f), arg1))
#define show_type_function_fromparams_2(f, arg1, arg2) type_name::internal::lambda_to_fn(\
  tn_type_lamda_generic_fromparams_2(type_name::internal::wrap_function(f), arg1, arg2))
#define show_type_function_fromparams_3(f, arg1, arg2, arg3) type_name::internal::lambda_to_fn(\
  tn_type_lamda_generic_fromparams_3(type_name::internal::wrap_function(f), arg1, arg2, arg3))
#define show_type_function_fromparams_4(f, arg1, arg2, arg3, arg4) type_name::internal::lambda_to_fn(\
  tn_type_lamda_generic_fromparams_4(type_name::internal::wrap_function(f), arg1, arg2, arg3, arg4))
#define show_type_function_fromparams_5(f, arg1, arg2, arg3, arg4, arg5) type_name::internal::lambda_to_fn(\
  tn_type_lamda_generic_fromparams_5(type_name::internal::wrap_function(f), arg1, arg2, arg3, arg4, arg5))

#define show_type_function_fromtypes_1(f, arg1) type_name::internal::lambda_to_fn(\
  show_type_lambda_generic_fromtypes_1(type_name::internal::wrap_function(f), arg1))
#define show_type_function_fromtypes_2(f, arg1, arg2) type_name::internal::lambda_to_fn(\
  show_type_lambda_generic_fromtypes_2(type_name::internal::wrap_function(f), arg1, arg2))
#define show_type_function_fromtypes_3(f, arg1, arg2, arg3) type_name::internal::lambda_to_fn(\
  show_type_lambda_generic_fromtypes_3(type_name::internal::wrap_function(f), arg1, arg2, arg3))
#define show_type_function_fromtypes_4(f, arg1, arg2, arg3, arg4) type_name::internal::lambda_to_fn(\
  show_type_lambda_generic_fromtypes_4(type_name::internal::wrap_function(f), arg1, arg2, arg3, arg4))
#define show_type_function_fromtypes_5(f, arg1, arg2, arg3, arg4, arg5) type_name::internal::lambda_to_fn(\
  show_type_lambda_generic_fromtypes_5(type_name::internal::wrap_function(f), arg1, arg2, arg3, arg4, arg5))

#endif // #ifndef _MSC_VER