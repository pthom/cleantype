// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#include "doctest.h"
#include <cleantype/cleantype.hpp>
#include <cleantype/details/debug_break.hpp>
#include <fplus/fplus.hpp>
#include <functional>
#include <map>

#define LOG(str) std::cout << str << std::endl

TEST_CASE("CT_show_details")
{
    {
        int a = 1;
        REQUIRE_EQ(CT_show_details(a), "[int] a = 1");
    }
    {
        std::string s("hello");
        REQUIRE_EQ(CT_show_details(s), "[std::string] s = hello");
    }
    {
        std::vector<int> v{{1, 2, 3, 4, 5}};
        REQUIRE_EQ(CT_show_details_cont(v), "[std::vector<int>] v = [1, 2, 3, 4, 5]");
    }
    {
        std::map<std::string, int> v{{{"a", 1}, {"b", 2}, {"c", 3}}};
        auto t = CT_show_details_cont(v);
        REQUIRE_EQ(CT_show_details_cont(v),
                   "[std::map<std::string, int>] v = [(a, 1), (b, 2), (c, 3)]");
    }
    {
        std::vector<std::string> v{{{"a"}, {"b"}, {"c"}}};
        REQUIRE_EQ(CT_show_details(v), "[std::vector<std::string>] v = [a, b, c]");
    }
    {
        std::set<std::string> v{{{"a"}, {"b"}, {"c"}}};
        REQUIRE_EQ(CT_show_details(v), "[std::set<std::string>] v = [a, b, c]");
    }
}

template <typename Transform>
auto make_test_string_transform(Transform f)
{
    return [f](std::string const &input, std::string const &expected_output) {
        std::string computed_output = f(input);
        if (computed_output != expected_output)
            std::cout << "Mince";
        REQUIRE_EQ(computed_output, expected_output);
    };
}

TEST_CASE("clean_from_values")
{
    int a = 3;
    int const &b = a;
    int const *const c = &a;
    char d = 42;
    std::vector<std::string> ee;
    const auto &e = ee;
    auto computed = cleantype::clean(a, b, c, d, e);
    auto expected = std::string(
        "int &, int const &, int const * const &, char &, std::vector<std::string> const &");
    REQUIRE_EQ(computed, expected);
}

TEST_CASE("clean_typename_from_string")
{
    auto make_one_test = make_test_string_transform(cleantype::clean_typestring);
    make_one_test("  int   ", "int");
    make_one_test("int&", "int &");
    make_one_test("int&&", "int &&");
    make_one_test("std::pair<int,double> ", "std::pair<int, double>");
    make_one_test("std::pair<int, double> ", "std::pair<int, double>");
    make_one_test("std::pair < std::vector< std::string,int > >",
                  "std::pair<std::vector<std::string, int>>");
    make_one_test("std::pair < std::vector< std::string > >",
                  "std::pair<std::vector<std::string>>");
    make_one_test("std::__1::basic_string<char> const &", "std::string const &");
    make_one_test("int&", "int &");
    make_one_test("std::string const&", "std::string const &");
    make_one_test(
        "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > "
        "const &",
        "std::string const &");
    make_one_test("class std::vector<int,class std::allocator<int> > const&",
                  "std::vector<int> const &");
}

void compare_type_full_to_repr(std::string const &type_full, std::string const &expected_repr)
{
    std::string type_clean = cleantype::clean_typestring(type_full);
    std::string type_west = cleantype::apply_east_const_typelist(type_clean);
    std::string expected_repr2 =
        cleantype_fp::replace_tokens(std::string(" COMMA "), std::string(", "), expected_repr);
    if (type_west != expected_repr2)
        std::cout << "Ah";
    REQUIRE_EQ(type_west, expected_repr2);
}

template <typename T>
void impl_test_clean_type(std::string const &expectedRepr, T value)
{
    std::string type_full = CT_cleantype_full(value);
    compare_type_full_to_repr(type_full, expectedRepr);
}

#define test_clean_type__defaultcontructible(type_definition)                              \
    {                                                                                      \
        type_definition value = type_definition();                                         \
        impl_test_clean_type<type_definition>(#type_definition, value);                    \
        impl_test_clean_type<type_definition const &>(#type_definition " const &", value); \
    }

#define COMMA ,

TEST_CASE("clean_typename_from_type")
{
    test_clean_type__defaultcontructible(int);
    test_clean_type__defaultcontructible(std::vector<int>);
    test_clean_type__defaultcontructible(std::pair<int COMMA int>);
    test_clean_type__defaultcontructible(
        std::deque<std::pair<std::string COMMA std::map<int COMMA int>>>);
    test_clean_type__defaultcontructible(std::set<int>);
    test_clean_type__defaultcontructible(std::list<std::string>);
}

TEST_CASE("clean_pack")
{
    REQUIRE_EQ(cleantype::clean<std::string>(), "std::string");
    REQUIRE_EQ(cleantype::clean<std::string, std::vector<int>>(), "std::string, std::vector<int>");
    REQUIRE_EQ(cleantype::clean<std::string, const std::vector<int> &, int const &, int &&>(),
               "std::string, std::vector<int> const &, int const &, int &&");
}

TEST_CASE("clean_multiple_args")
{
    REQUIRE_EQ(cleantype::clean(std::string("ah"), 1), "std::string, int");
    std::vector<int> v;
    REQUIRE_EQ(cleantype::clean(std::string("ah"), &v, 1), "std::string, std::vector<int> *, int");
}

TEST_CASE("impl_clean")
{
    std::string typ_name =
        "std::__1::map<int, std::__1::vector<int, std::__1::allocator<int>>, std::__1::less<int>, "
        "std::__1::allocator<std::__1::pair<int const, std::__1::vector<int, "
        "std::__1::allocator<int>>>>>";
    std::string type_cleaned = cleantype::clean_typestring(typ_name);
    std::string expected = "std::map<int, std::vector<int>>";
    REQUIRE_EQ(type_cleaned, expected);
}