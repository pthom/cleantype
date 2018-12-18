// This file is part of ConsType : Consistent Type names for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#include "doctest.h"
#include "constype/constype.hpp"
#include <fplus/fplus.hpp>
#include <functional>
#include <map>

#define LOG(str) std::cout << str << std::endl

template<typename Lambda> void test_one_lambda(Lambda f, const std::string & expected_type)
{
    const std::string computed_type = TN_show_details_lambda(f);
    if (computed_type != expected_type)
        std::cout << "Argh";
    REQUIRE_EQ(computed_type, expected_type);
}



TEST_CASE("log_type_lambda_clean")
{
  {
    auto f = []() { std::cout << "Hello"; };
    REQUIRE_EQ(TN_show_details_lambda(f), "[lambda: () -> void] f");
  }
  {
    auto f = []() { return 42u; };
    auto s = TN_show_details_lambda(f);
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
    using namespace constype::internal;
    {
        std::string params_str("int, string");
        auto params = tokenize_lambda_params(params_str, false);
        std::vector<std::string> expected {
            {"int"},
            {"string"}
        };
        REQUIRE_EQ(params, expected);
    }
    {
        std::string params_str("int, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, double");
        auto params = tokenize_lambda_params(params_str, false);
        std::vector<std::string> expected {
            {"int"},
            {"std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >"},
            {"double"},
        };
        REQUIRE_EQ(params, expected);
    }
    {
        std::string params_str("int, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, double");
        auto params = tokenize_lambda_params(params_str, true);
        std::vector<std::string> expected {
            {"int"},
            {"std::string"},
            {"double"},
        };
        REQUIRE_EQ(params, expected);
    }
    {
        std::string params_str("");
        auto params = tokenize_lambda_params(params_str, true);
        std::vector<std::string> expected { "" };
        REQUIRE_EQ(params, expected);
    }
    {
        std::string params_str("int");
        auto params = tokenize_lambda_params(params_str, true);
        std::vector<std::string> expected {
            "int"
        };
        REQUIRE_EQ(params, expected);
    }
    {
        std::string params_str("const std::__1::basic_string<char> &, const std::__1::basic_string<char> &");
        auto params = tokenize_lambda_params(params_str, true);
        std::vector<std::string> expected {
            "const std::string &", "const std::string &"
        };
        REQUIRE_EQ(params, expected);
    }
}

TEST_CASE("extract_parenthesis_content_at_end")
{
    std::string input = "ABC(DEF)(GHI)KLM";
    auto r = constype::internal::extract_parenthesis_content_at_end(input);
    REQUIRE_EQ(r.parenthesis_content, "GHI");
    REQUIRE_EQ(r.remaining_at_start, "ABC(DEF)");
    REQUIRE(r.success);
}


TEST_CASE("_mem_fn_to_lambda_type")
{
    {
        std::string memfn_type = "class std::_Mem_fn<struct std::pair<int,double> (__thiscall <lambda_e15113958de8c2368f6f706484d8ddc7>::*)(int,int)const >";
        std::string expected = "lambda: (int, int) -> std::pair<int, double>";
        auto computed = constype::internal::_mem_fn_to_lambda_type(memfn_type, true);
        REQUIRE_EQ(computed, expected);
    }
    {
        std::string memfn_type = "std::__1::__mem_fn<std::__1::map<int, std::__1::vector<int, std::__1::allocator<int>>, std::__1::less<int>, std::__1::allocator<std::__1::pair<int const, std::__1::vector<int, std::__1::allocator<int>>>>>(_DOCTEST_ANON_FUNC_2()::$_5:: *)(int)const>";
        std::string expected = "lambda: (int) -> std::map<int, std::vector<int>>";
        auto computed = constype::internal::_mem_fn_to_lambda_type(memfn_type, true);
        REQUIRE_EQ(computed, expected);
    }
    {
        // PERAVE                                                                  \/ !!!
//        std::string memfn_type = "std::__1::__mem_fn<std::__1::basic_string<char, char> ((lambda at ../src/include/constype/tests/lambda_typename_test.cpp:37:16):: *)(const std::__1::basic_string &, const std::__1::basic_string<char> &)const>";
//        std::string expected = "lambda: (const std::string &, const std::string &) -> std::string";
//        auto computed = constype::internal::_mem_fn_to_lambda_type(memfn_type, true);
//        REQUIRE_EQ(computed, expected);
    }
}