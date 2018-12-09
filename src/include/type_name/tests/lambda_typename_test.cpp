#include "doctest.h"
#include "type_name/type_name.hpp"
#include <fplus/fplus.hpp>
#include <functional>
#include <map>

#define LOG(str) std::cout << str << std::endl

template<typename Lambda> void test_one_lambda(Lambda f, const std::string & expected_type)
{
    const std::string computed_type = tn_show_details_lambda(f);
    REQUIRE_EQ(computed_type, expected_type);
}



TEST_CASE("log_type_lambda_clean")
{
  {
    auto f = []() { std::cout << "Hello"; };
    REQUIRE_EQ(tn_show_details_lambda(f), "[lambda: () -> void] f");
  }
  {
    auto f = []() { return 42u; };
    auto s = tn_show_details_lambda(f);
    test_one_lambda(f, "[lambda: () -> unsigned int] f");
  }
  {
    int c = 5;
    auto f = [&c](int a, int b) -> double { return a + b + c; };
    test_one_lambda(f, "[lambda: (int, int) -> double] f");
  }
  {
      auto f = [](std::string const &a, const std::string & b) { return a + b; };
      test_one_lambda(f, "[lambda: (std::string const &, std::string const &) -> std::string] f");
  }
  {
    int c = 5;
    auto f = [](int a, int b) { return std::pair<int, double>(a + b, cos(a + static_cast<double>(b))); };
    test_one_lambda(f, "[lambda: (int, int) -> std::pair<int, double>] f");
  }
  {
    std::string prefix = "a-";
    auto f = [&prefix](const std::string &s) { return prefix + s; };
    test_one_lambda(f, "[lambda: (std::string const &) -> std::string] f");
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

TEST_CASE("extract_parenthesis_content_at_end")
{
    std::string input = "ABC(DEF)(GHI)KLM";
    auto r = type_name::internal::extract_parenthesis_content_at_end(input);
    REQUIRE_EQ(r.parenthesis_content, "GHI");
    REQUIRE_EQ(r.remaining_at_start, "ABC(DEF)");
    REQUIRE(r.success);
}


TEST_CASE("_mem_fn_to_lambda_type")
{
  {
    std::string memfn_type = "class std::_Mem_fn<struct std::pair<int,double> (__thiscall <lambda_e15113958de8c2368f6f706484d8ddc7>::*)(int,int)const >";
    std::string expected = "[lambda: (int, int) -> std::pair<int, double>]";
    //                      "lambda: (int, int) -> std::pair<int,double>"
    auto computed = type_name::internal::_mem_fn_to_lambda_type(memfn_type, true);
    //LOG(computed);
  }
}