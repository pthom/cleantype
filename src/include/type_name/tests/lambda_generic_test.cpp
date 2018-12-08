#include "doctest.h"
#include "type_name/type_name_clean.hpp"
#include "type_name/details/lambda_typename.hpp"
#include <fplus/fplus.hpp>
#include <functional>
#include <map>

#define LOG(str) std::cout << str << std::endl

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
        std::string mem_fn_type = var_type_name_full(as_mem_fn);
        bool clean = true;
        type_name = type_name::internal::_mem_fn_to_lambda_type(mem_fn_type, clean);
      }
    };
  }
}


// NAMING = show_details / show_details_lambda / show_type_lambda_generic_1 etc ...


#define show_type_lambda_generic_1(fn, arg1) type_name::internal::lambda_generic_type_holder<decltype(fn), decltype(arg1)>().type_name
#define show_type_lambda_generic_2(fn, arg1, arg2) type_name::internal::lambda_generic_type_holder<decltype(fn), decltype(arg1), decltype(arg2)>().type_name
#define show_type_lambda_generic_3(fn, arg1, arg2, arg3) type_name::internal::lambda_generic_type_holder<decltype(fn), decltype(arg1), decltype(arg2), decltype(arg3)>().type_name

#define show_details_lambda_generic_1(fn, arg1) std::string("[") + show_type_lambda_generic_1(fn, arg1) + "] " + #fn
#define show_details_lambda_generic_2(fn, arg1, arg2) std::string("[") + show_type_lambda_generic_2(fn, arg1, arg2) + "] " + #fn
#define show_details_lambda_generic_3(fn, arg1, arg2, arg3) std::string("[") + show_type_lambda_generic_3(fn, arg1, arg2, arg3) + "] " + #fn


TEST_CASE("log_type_lambda_clean___compose")
{

  {
    auto my_square = [](int a) { return a * a; };
    auto my_double = [](int a) { return a * 2; };
    auto f = fplus::fwd::compose(my_square, my_double);

    auto double_auto = [](auto x) { return 2. * x; };

    //using namespace type_name;

    // debug variadic macros : https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html
    // LOG(v2_type_lambda_generic(f, int));

    auto f4 = [](auto x, auto y) {
      return x + y;
    };

    LOG(show_details_lambda_generic_1(f, 4));
    LOG(show_details_lambda_generic_2(f4, 3, 4.));
  }
}

int my_function(int a, int b)
{
  return a + b;
}

auto wrap_function = [](auto && f) {
  return [&](auto... args) {
    return f(args...);
  };
};

TEST_CASE("log_wrap_function")
{
  auto f_log_wrap_function = wrap_function(my_function);
  //LOG(ff(1, 2));
  //LOG(show_details_lambda(ff));
  LOG(show_details_lambda_generic_2(f_log_wrap_function, 2, 3));
}


// TODO :
// - finish variadic lambdas
// - type of polymorphic lambdas
// - type of functions (wrap in std::function ?)

