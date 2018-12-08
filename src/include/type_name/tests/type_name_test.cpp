#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "type_name/type_name_clean.hpp"
#include "type_name/details/lambda_typename.hpp"
#include <fplus/fplus.hpp>
#include <functional>
#include <map>



TEST_CASE("log_var")
{
    {
        int a = 1;
        REQUIRE_EQ(log_var_str(a), "[int] a = 1");
    }
    {
        std::string s("hello");
        REQUIRE_EQ(log_var_str(s), "[std::string] s = hello");
    }
    {
        std::vector<int> v{{ 1, 2, 3, 4, 5}};
        REQUIRE_EQ(log_var_str(v), "[std::vector<int>] v = [1, 2, 3, 4, 5]");
    }
    {
        std::map<std::string, int> v {{
            {"a", 1},
            {"b", 2},
            {"c", 3}
        }};
         REQUIRE_EQ(log_var_str_cont(v), "[std::map<std::string, int>] v = [(a, 1), (b, 2), (c, 3)]");
    }
    {
        std::vector<std::string> v {{
            {"a"},
            {"b"},
            {"c"}
        }};
         REQUIRE_EQ(log_var_str(v), "[std::vector<std::string>] v = [a, b, c]");
    }
    {
        std::set<std::string> v {{
            {"a"},
            {"b"},
            {"c"}
        }};
         REQUIRE_EQ(log_var_str(v), "[std::set<std::string>] v = [a, b, c]");
    }
}


TEST_CASE("log_type_lambda_clean")
{
    {
        auto f = [](){ std::cout << "Hello"; };
        REQUIRE_EQ(log_type_lambda_clean_str(f), "[lambda: () -> void] f");
    }
    {
        auto f = [](){ return 42u; };
        REQUIRE_EQ(log_type_lambda_clean_str(f), "[lambda: () -> unsigned int] f");
    }
    {
        int c = 5;
        auto f = [&c](int a, int b) -> double { return a + b + c; };
        REQUIRE_EQ(log_type_lambda_clean_str(f), "[lambda: (int, int) -> double] f");
    }
    {
        int c = 5;
        auto f = [](int a, int b)  { return std::pair<int, double>(a + b, cos(a + static_cast<double>(b))); };
        REQUIRE_EQ(log_type_lambda_clean_str(f), "[lambda: (int, int) -> std::pair<int, double>] f");
    }
    {
        std::string prefix = "a-";
        auto f = [&prefix](const std::string &s)  { return prefix + s; };
        REQUIRE_EQ(log_type_lambda_clean_str(f), "[lambda: (std::string& const) -> std::string] f");
    }
}


namespace type_name
{
    template <typename LambdaFunction, typename... Args>
    std::string type_lambda_variadic(LambdaFunction fn, bool clean)
    {
        auto as_mem_fn = std::mem_fn( & LambdaFunction::template operator()<Args...> );
        std::string mem_fn_type = var_type_name_full(as_mem_fn);
        return internal::_mem_fn_to_lambda_type(mem_fn_type, clean);
    }

    // std::string make_type_lambda_variadic()
    auto make_type_lambda_variadic = [](auto f) {

    };
}


TEST_CASE("log_type_lambda_clean___compose")
{

    {
        auto my_square = [](int a) { return a * a;};
        auto my_double = [](int a) { return a * 2; };
        auto f = fplus::fwd::compose(my_square, my_double);
        //log_type_lambda_clean_str(f);
        //std::cout << type_name::type_lambda_variadic<decltype(f), int>(f, true) << std::endl;
    }
}


// TODO :
// - finish variadic lambdas
// - type of polymorphic lambdas
// - type of functions (wrap in std::function ?)




////////////// Internal tests below

TEST_CASE("tokenize_lambda_params")
{
    using namespace type_name::internal;
    {
        std::string params_str("int, string");
        auto params = tokenize_lambda_params(params_str, false);
        std::vector<std::string> expected {{
            {"int"},
            {"string"}
        }};
        REQUIRE_EQ(params, expected);
    }
    {
        std::string params_str("int, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, double");
        auto params = tokenize_lambda_params(params_str, false);
        std::vector<std::string> expected {{
            {"int"},
            {"std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >"},
            {"double"},
        }};
        REQUIRE_EQ(params, expected);
    }
    {
        std::string params_str("int, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, double");
        auto params = tokenize_lambda_params(params_str, true);
        std::vector<std::string> expected {{
            {"int"},
            {"std::string"},
            {"double"},
        }};
        REQUIRE_EQ(params, expected);
    }
    {
        std::string params_str("");
        auto params = tokenize_lambda_params(params_str, true);
        std::vector<std::string> expected {{
        }};
        REQUIRE_EQ(params, expected);
    }
    {
        std::string params_str("int");
        auto params = tokenize_lambda_params(params_str, true);
        std::vector<std::string> expected {{
            "int"
        }};
        REQUIRE_EQ(params, expected);
    }
}


TEST_CASE("demangle_test_fixme")
{
    // {
    //     const std::vector<std::pair<std::string, int>> v;
    //     //const auto &&vv = std::move(v);
    //     std::string type_full = var_type_name_full(v);
    //     std::string type_clean = type_name::demangle_typename(type_full);
    //     std::cout << "type_full: " << type_full << "\n";
    //     std::cout << "type_clean: " << type_clean << '\n';
    // }
    // {
    //     std::cout << "\n";
    //     const std::vector<std::pair<std::string, int>> v;
    //     const auto &&vv = std::move(v);
    //     auto s = var_type_name_full(vv);
    //     std::cout << "Mangled:\n"
    //               << s << "\n";
    //     std::cout << "\n";
    //     std::cout << "Demangled:\n"
    //               << type_name::demangle_typename(s) << '\n';
    // }

    // {
    //     std::cout << "\n";
    //     auto f = [](const std::vector<std::string> &v, int a) {
    //         return v.size() + a;
    //     };
    //     auto s = var_type_name_full(f);
    //     std::cout << "Mangled:\n"
    //               << s << "\n";
    //     std::cout << "\n";
    //     std::cout << "Demangled:\n"
    //               << type_name::demangle_typename(s) << '\n';
    // }

}
