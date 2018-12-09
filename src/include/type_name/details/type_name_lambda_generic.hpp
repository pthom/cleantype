#include <type_name/type_name.hpp>

namespace type_name
{
  namespace internal
  {
    template <typename LambdaFunction, typename... Args>
    struct lambda_generic_type_holder {
      std::string type_name;
      lambda_generic_type_holder() {
#ifdef _MSC_VER
        auto ptr = &LambdaFunction::operator() < Args... > ;
#else
        auto ptr = &LambdaFunction::template operator() < Args... > ;
#endif
        auto as_mem_fn = std::mem_fn(ptr);

        // auto as_mem_fn = std::mem_fn(
        //     & LambdaFunction::template operator()<Args...> );

        std::string mem_fn_type = type_name_full(as_mem_fn);
        bool clean = true;
        type_name = type_name::internal::_mem_fn_to_lambda_type(mem_fn_type, clean);
      }
    };
  }
}


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
