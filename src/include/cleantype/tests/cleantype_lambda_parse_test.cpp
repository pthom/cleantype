// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#include "doctest.h"
#include <cleantype/cleantype.hpp>
#include <cleantype/details/debug_break.hpp>
#include <fplus/fplus.hpp>
#include <functional>
#include <map>

// #define LOG(str) std::cout << str << std::endl
// #define LOG_VALUE(v) std::cout << #v << "==>" << v << "<==" << std::endl


TEST_CASE("tokenize_params_around_comma")
{
    using namespace cleantype::internal;
    {
        std::string params_str("int, string");
        auto params = tokenize_params_around_comma(params_str, false);
        std::vector<std::string> expected {
            {"int"},
            {"string"}
        };
        REQUIRE_EQ(params, expected);
    }
    {
        std::string params_str("int, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, double");
        auto params = tokenize_params_around_comma(params_str, false);
        std::vector<std::string> expected {
            {"int"},
            {"std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >"},
            {"double"},
        };
        REQUIRE_EQ(params, expected);
    }
    {
        std::string params_str("int, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, double");
        auto params = tokenize_params_around_comma(params_str, true);
        std::vector<std::string> expected {
            {"int"},
            {"std::string"},
            {"double"},
        };
        REQUIRE_EQ(params, expected);
    }
    {
        std::string params_str("");
        auto params = tokenize_params_around_comma(params_str, true);
        std::vector<std::string> expected { "" };
        REQUIRE_EQ(params, expected);
    }
    {
        std::string params_str("int");
        auto params = tokenize_params_around_comma(params_str, true);
        std::vector<std::string> expected {
            "int"
        };
        REQUIRE_EQ(params, expected);
    }
    {
        std::string params_str("std::__1::basic_string<char> const &, std::__1::basic_string<char> const &");
        auto params = tokenize_params_around_comma(params_str, true);
        std::vector<std::string> expected {
            "std::string const &", "std::string const &"
        };
        REQUIRE_EQ(params, expected);
    }
}

TEST_CASE("extract_parenthesis_content_at_end")
{
    std::string input = "ABC(DEF)(GHI)KLM";
    auto r = cleantype::internal::extract_parenthesis_content_at_end(input);
    REQUIRE_EQ(r.parenthesis_content, "GHI");
    REQUIRE_EQ(r.remaining_at_start, "ABC(DEF)");
    REQUIRE(r.success);
}


TEST_CASE("_mem_fn_to_lambda_type")
{
    {
        std::string memfn_type = "class std::_Mem_fn<struct std::pair<int,double> (__thiscall <lambda_e15113958de8c2368f6f706484d8ddc7>::*)(int,int)const >";
        std::string expected = "lambda: (int, int) -> std::pair<int, double>";
        auto computed = cleantype::internal::_mem_fn_to_lambda_type(memfn_type, true);
        REQUIRE_EQ(computed, expected);
    }
    {
        std::string memfn_type = "std::__1::__mem_fn<std::__1::map<int, std::__1::vector<int, std::__1::allocator<int>>, std::__1::less<int>, std::__1::allocator<std::__1::pair<int const, std::__1::vector<int, std::__1::allocator<int>>>>>(_DOCTEST_ANON_FUNC_2()::$_5:: *)(int)const>";
        std::string expected = "lambda: (int) -> std::map<int, std::vector<int>>";
        auto computed = cleantype::internal::_mem_fn_to_lambda_type(memfn_type, true);
        REQUIRE_EQ(computed, expected);
    }
}
