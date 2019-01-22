// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)

#include "debug_utilities.hpp"
#include "doctest.h"
#include <cleantype/details/cleantype_full.hpp>
#include <regex>

#ifdef _WIN32
#include <Windows.h>
#endif  // _WIN32

void my_require_eq_string(std::string const & computed, std::string const & expected)
{
#ifdef _MSC_VER  // remove __ptr64 from msvc types
    std::string computed2 = cleantype_fp::replace_tokens(" __ptr64", "", computed);
    std::string expected2 = cleantype_fp::replace_tokens(" __ptr64", "", expected);
    if (computed2 != expected2)
        DEBUGBREAK;
    REQUIRE_EQ(computed2, expected2);
#else
    if (computed != expected)
        DEBUGBREAK;
    REQUIRE_EQ(computed, expected);
#endif
}

#ifndef _MSC_VER
#define VOLATILE_CHAR "volatile char"
#else
#define VOLATILE_CHAR "char volatile"
#endif

TEST_CASE("cleantype_full_test_s")
{
    {
        // Standard types
        char v = 5;
        my_require_eq_string(cleantype::full(v), "char &");
        // The macro will output the exact type
        my_require_eq_string(CT_cleantype_full(v), "char");
    }
    {
        // Reference
        char a = 5;
        char & v = a;
        my_require_eq_string(cleantype::full(v), "char &");
        my_require_eq_string(CT_cleantype_full(v), "char &");
    }
    {
        // Const reference
        char a = 5;
        const char & v = a;
        my_require_eq_string(cleantype::full(v), "char const &");
        my_require_eq_string(cleantype::apply_east_const_typelist(CT_cleantype_full(v)),
                             "char const &");
    }
    {
        // Pointer to const
        char a = 5;
        const char * v = &a;
        my_require_eq_string(cleantype::full(v), "char const * &");
        my_require_eq_string(cleantype::apply_east_const_typelist(CT_cleantype_full(v)),
                             "char const *");
    }
    {
        // Const pointer (but modifiable content)
        char a = 5;
        const char * v = &a;
        my_require_eq_string(cleantype::full(v), "char const * &");
        my_require_eq_string(cleantype::apply_east_const_typelist(CT_cleantype_full(v)),
                             "char const *");
    }
    {
        // Volatile
        volatile char v = 5;
        my_require_eq_string(cleantype::full(v), VOLATILE_CHAR " &");
        my_require_eq_string(CT_cleantype_full(v), VOLATILE_CHAR);
    }
}

TEST_CASE("cleantype_full_r_value_references")
{
    using TypenamePair = std::array<std::string, 2>;
    // r-value reference tests
    auto output_received_type = [](auto && v) -> TypenamePair {
        return {cleantype::full<decltype(v)>(), cleantype::full(v)};
    };
    auto require_eq_typename_pair = [](const TypenamePair & p1, const TypenamePair & p2) {
        if (p1[0] != p2[0])
            std::cout << "ARGGHHH" << std::endl;
        if (p1[1] != p2[1])
            std::cout << "ARGGHHH" << std::endl;
        my_require_eq_string(p1[0], p2[0]);
        my_require_eq_string(p1[1], p2[1]);
    };

    {
        // with a standard value
        char v = 5;
        require_eq_typename_pair(output_received_type(v), {"char &", "char &"});
    }
    // with a reference
    {
        char a = 5;
        char & v = a;
        require_eq_typename_pair(output_received_type(v), {"char &", "char &"});
    }
    // with a const reference
    {
        char a = 5;
        const char & v = a;
        require_eq_typename_pair(output_received_type(v), {"char const &", "char const &"});
    }
    // with an r-value reference
    {
        std::string s("hello there, how are you. This is not a short string");
        require_eq_typename_pair(output_received_type(static_cast<char>(42)),
                                 {"char &&", "char &"});
    }
}

TEST_CASE("CT_cleantype_full")
{
    {
        char v = 5;
        my_require_eq_string(CT_cleantype_full(v), "char");
    }
    {
        my_require_eq_string(CT_cleantype_full(static_cast<char>(42)), "char");
    }
}

template <typename... Args>
struct TemplateClass
{
    static std::string full_type() { return cleantype::full<Args...>(); }
};

template <typename... Args>
void check_multiple_args(std::string const & expected)
{
    auto v1 = cleantype::full<Args...>();
    auto v2 = TemplateClass<Args...>::full_type();
    if (v1 != expected)
        std::cout << "pb1\n";
    if (v2 != expected)
        std::cout << "pb2\n";
    my_require_eq_string(cleantype::full<Args...>(), expected);
    my_require_eq_string(TemplateClass<Args...>::full_type(), expected);
}

TEST_CASE("cleantype_full_multiple")
{
    check_multiple_args<char, const char>("char, char const");
    check_multiple_args<char &, const char &>("char &, char const &");
    check_multiple_args<char &&>("char &&");
    check_multiple_args<char &&>("char &&");

    check_multiple_args<char *, char const *, char * const, char const * const>(
        "char *, char const *, char * const, char const * const");
}

template <typename... Args>
void check_multiple_args_fromvalues(std::string const & expected, Args... args)
{
    my_require_eq_string(cleantype::full(args...), expected);
}

TEST_CASE("cleantype_full_multiple_fromvalues")
{
    my_require_eq_string(cleantype::full(static_cast<char>(1)), "char");
    my_require_eq_string(cleantype::full(static_cast<char>(1), static_cast<char>(1)), "char, char");
    {
        char a = 5;
        char & v = a;
        char & v2 = a;
        const char & c = a;
        my_require_eq_string(cleantype::full(a, v, c, c),
                             "char &, char &, char const &, char const &");
    }
}

template <typename... T>
struct Template
{
};

template <typename T>
void check_matches(std::string const & re)
{
    std::string name = cleantype::full<T>();
    std::regex regex{re};
    bool flag_regex_match = std::regex_match(name, regex);
    if (!flag_regex_match)
    {
        std::cerr << "type name '" << name << "' does not match regex '" << re << "'" << std::endl;
    }
    REQUIRE(flag_regex_match);
}

TEST_CASE("cleantype_full_regex")
{
    // Make sure we get something reasonable
    check_matches<int const>("int const|const int");
    check_matches<int &>(R"(int\s*&)");
    check_matches<int const &>(R"(const\s+int\s*&|int\s+const\s*&)");
#ifndef _MSC_VER
    check_matches<int(&)[]>(R"(int\s*\(\s*&\s*\)\s*\[\s*\])");
#endif
    check_matches<int(&)[10]>(R"(int\s*\(\s*&\s*\)\s*\[\s*10\s*\])");
    check_matches<Template<void, char const *>>(
        R"(Template<\s*void\s*,\s*(char const|const char)\s*\*\s*>)");
    check_matches<void (*)(int)>(R"(void\s*\(\s*\*\s*\)\s*\(\s*int\s*\))");
}

#ifdef _HANA_CT_CAN_CONSTEXPR
#define RUN_ONE_TYPE_TEST_COMPILE_TIME(type_definition, type_string_literal)                \
    {                                                                                       \
        constexpr auto computed =                                                           \
            cleantype::internal::_impl_typeid_hana_inside_holder<type_definition>();        \
        static_assert(boost::hana::experimental::type_name_details::stringliteral_equal_sz( \
                          computed, type_string_literal),                                   \
                      "RUN_ONE_TYPE_TEST_COMPILE_TIME error");                              \
    }
#else
#define RUN_ONE_TYPE_TEST_COMPILE_TIME(type_definition, type_string_literal)
#endif

#define RUN_ONE_TYPE_TEST_RUN_TIME(type_definition, type_string_literal)                         \
    {                                                                                            \
        auto computed = cleantype::internal::_impl_typeid_hana_inside_holder<type_definition>(); \
        auto computed_s =                                                                        \
            boost::hana::experimental::type_name_details::stringliteral_to_string(computed);     \
        std::cout << "computed:" << computed_s << std::endl;                                     \
        REQUIRE_EQ(computed_s, type_string_literal);                                             \
    }

void compile_time_tests()
{
    RUN_ONE_TYPE_TEST_RUN_TIME(void, "cleantype::internal::TupleTypeHolder<void>");
    RUN_ONE_TYPE_TEST_RUN_TIME(char, "cleantype::internal::TupleTypeHolder<char>");

    // __PRETTY_FUNCTION__ seems to favor west-const (this is true for MSVC, GCC and Clang)
    // on the contrary, typeid().name() is strictly east const accross all compilers
    // (Does this really need to be tested ?)
    RUN_ONE_TYPE_TEST_RUN_TIME(const char, "cleantype::internal::TupleTypeHolder<const char>");
    RUN_ONE_TYPE_TEST_RUN_TIME(char const, "cleantype::internal::TupleTypeHolder<const char>");
}
