#include "doctest.h"
#include "type_name/type_name_clean.hpp"
#include "type_name/details/lambda_typename.hpp"
#include <fplus/fplus.hpp>
#include <functional>
#include <map>

#define LOG(str) std::cout << str << std::endl

TEST_CASE("log_type_lambda_clean")
{
  {
    auto f = []() { std::cout << "Hello"; };
    REQUIRE_EQ(show_details_lambda(f), "[lambda: () -> void] f");
  }
  {
    auto f = []() { return 42u; };
    auto s = show_details_lambda(f);
    REQUIRE_EQ(show_details_lambda(f), "[lambda: () -> unsigned int] f");
  }
  {
    int c = 5;
    auto f = [&c](int a, int b) -> double { return a + b + c; };
    REQUIRE_EQ(show_details_lambda(f), "[lambda: (int, int) -> double] f");
  }
  {
    int c = 5;
    auto f = [](int a, int b) { return std::pair<int, double>(a + b, cos(a + static_cast<double>(b))); };
#ifndef _MSC_VER // missing space after ','
    REQUIRE_EQ(show_details_lambda(f), "[lambda: (int, int) -> std::pair<int, double>] f");
#endif
  }
  {
    std::string prefix = "a-";
    auto f = [&prefix](const std::string &s) { return prefix + s; };
    std::string expected1("[lambda: (std::string& const) -> std::string] f");
    std::string expected2("[lambda: (std::string & const) -> std::string] f");
    std::string computed = show_details_lambda(f);
    REQUIRE(((computed == expected1) || (computed == expected2)));
    // REQUIRE_EQ(show_details_lambda(f), "[lambda: (std::string& const) -> std::string] f");
  }
}


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

