// This file is part of ConsType : Consistent Type names for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#include "doctest.h"
#include <regex>
#include "constype/details/constype_full.hpp"

//#define LOG(str) std::cout << str << std::endl
//#define LOG_VALUE(var) std::cout << #var << " = " << var << std::endl

#ifdef _MSC_VER // remove __ptr64 from msvc types
#define MY_REQUIRE_EQ_STRING(a, b) \
{\
    std::string a2 = fp::replace_tokens(" __ptr64", "", a);\
    std::string b2 = fp::replace_tokens(" __ptr64", "", b);\
    REQUIRE_EQ(a2, b2);\
}
#else
#define MY_REQUIRE_EQ_STRING(a, b) REQUIRE_EQ(a, b)
#endif


TEST_CASE("ERROR_full")
{
    //constype::ERROR_full<int>();
}

TEST_CASE("constype_full_test_s")
{
    {
        // Standard types
        char v = 5;
        MY_REQUIRE_EQ_STRING(constype::full(v),
        "char &"
        );
        // The macro will output the exact type
        MY_REQUIRE_EQ_STRING(TN_constype_full(v),
        "char"
        );
    }
    {
        // Reference
        char a = 5;
        char &v = a;
        MY_REQUIRE_EQ_STRING(constype::full(v),
        "char &"
        );
        MY_REQUIRE_EQ_STRING(TN_constype_full(v),
        "char &"
        );
    }
    {
        // Const reference
        char a = 5;
        const char &v = a;
        MY_REQUIRE_EQ_STRING(constype::full(v),
        "const char &"
        );
        MY_REQUIRE_EQ_STRING(TN_constype_full(v),
        "const char &"
        );
    }
    {
        // Pointer to const
        char a = 5;
        const char *v = &a;
        MY_REQUIRE_EQ_STRING(constype::full(v),
        "const char * &"
        );
        MY_REQUIRE_EQ_STRING(TN_constype_full(v),
        "const char *"
        );
    }
    {
        // Const pointer (but modifiable content)
        char a = 5;
        char * const v = &a;
        MY_REQUIRE_EQ_STRING(constype::full(v),
        "char * const &"
        );
        MY_REQUIRE_EQ_STRING(TN_constype_full(v),
        "char * const"
        );
    }
    {
        // Volatile
        volatile char v = 5;
        MY_REQUIRE_EQ_STRING(constype::full(v),
        "volatile char &"
        );
        MY_REQUIRE_EQ_STRING(TN_constype_full(v),
        "volatile char"
        );
    }
}


TEST_CASE("constype_full_r_value_references")
{
    using TypenamePair = std::array<std::string, 2>;
    // r-value reference tests
    auto output_received_type = [](auto && v) -> TypenamePair {
        return { constype::full<decltype(v)>(), constype::full(v) };
    };
    auto require_eq_typename_pair = [](const TypenamePair & p1, const TypenamePair & p2 ) {
        if (p1[0] != p2[0])
            std::cout << "ARGGHHH" << std::endl;
        if (p1[1] != p2[1])
            std::cout << "ARGGHHH" << std::endl;
        MY_REQUIRE_EQ_STRING(p1[0], p2[0]);
        MY_REQUIRE_EQ_STRING(p1[1], p2[1]);
    };

    {
        // with a standard value
        char v = 5;
        require_eq_typename_pair(output_received_type(v),
        { "char &", "char &"}
        );
    }
    // with a reference
    {
        char a = 5;
        char &v = a;
        require_eq_typename_pair(output_received_type(v),
        { "char &", "char &"}
        );
    }
    // with a const reference
    {
        char a = 5;
        const char &v = a;
        require_eq_typename_pair(output_received_type(v),
        { "const char &", "const char &"}
        );
    }
    // with an r-value reference
    {
        std::string s("hello there, how are you. This is not a short string");
        require_eq_typename_pair(output_received_type(static_cast<char>(42)),
        { "char &&", "char &" }
        );
    }

}


 TEST_CASE("TN_constype_full")
 {
     {
         char v = 5;
         MY_REQUIRE_EQ_STRING(
             TN_constype_full(v),
             "char"
          );
     }
     {
         MY_REQUIRE_EQ_STRING(
             TN_constype_full(static_cast<char>(42)),
             "char"
          );
     }
 }


 template<typename... Args>
 struct TemplateClass
 {
     static std::string full_type() {
         return constype::full<Args...>();
     }
 };


 template<typename... Args>
 void check_multiple_args(const std::string & expected)
 {
     auto v1 = constype::full<Args...>();
     auto v2 = TemplateClass<Args...>::full_type();
     if (v1 != expected)
         std::cout << "pb1\n";
     if (v2 != expected)
         std::cout << "pb2\n";
     MY_REQUIRE_EQ_STRING(
         constype::full<Args...>(),
         expected
     );
     MY_REQUIRE_EQ_STRING(
         TemplateClass<Args...>::full_type(),
         expected
     );
 }



TEST_CASE("constype_full_multiple")
{
     check_multiple_args<
          char, const char>(
         "char, const char");
     check_multiple_args<
          char &, const char & >(
         "char &, const char &"
     );
     check_multiple_args<
          char &&>(
         "char &&"
     );
     check_multiple_args<
          char &&>(
         "char &&"
     );

     check_multiple_args<
          char *, const char *, char * const >(
         "char *, const char *, char * const"
     );
 }


 template<typename... Args>
 void check_multiple_args_fromvalues(const std::string & expected, Args... args)
 {
     MY_REQUIRE_EQ_STRING(
         constype::full(args...),
         expected
     );
 }


TEST_CASE("constype_full_multiple_fromvalues")
{
     MY_REQUIRE_EQ_STRING(
         constype::full(static_cast<char>(1)),
         "char"
     );
     MY_REQUIRE_EQ_STRING(
         constype::full(static_cast<char>(1), static_cast<char>(1)),
         "char, char"
     );
     std::string tst = constype::full(static_cast<char>(1), "hello");
 #ifndef _MSC_VER // msvc emits "char, const char[6] const &"
     MY_REQUIRE_EQ_STRING(
         constype::full(static_cast<char>(1), "hello"),
         "char, char const(&)[6]"
     );
 #endif
     {
         // Three params and perfect forwarding -> fail !
         // This fails because it is on the third param
         // See possible leads:
         // http://www.cplusplus.com/reference/tuple/forward_as_tuple/
         // +
         // https://stackoverflow.com/questions/1198260/iterate-over-tuple
         char a = 5;
         char &v = a;
         char &v2 = a;
         const char& c = a;
         MY_REQUIRE_EQ_STRING(constype::full(a, v, c, c),
          "char &, char &, const char &, const char &"
          );
     }
}

template<typename... T>
struct Template {
};


template <typename T>
void check_matches(std::string const& re) {
    std::string name = constype::full<T>();
    std::regex regex{re};
    bool flag_regex_match = std::regex_match(name, regex);
    if (!flag_regex_match) {
        std::cerr << "type name '" << name << "' does not match regex '" << re << "'" << std::endl;
    }
    REQUIRE(flag_regex_match);
}


TEST_CASE("constype_full_regex")
{
    // Make sure we get something reasonable
    check_matches<int const>("int const|const int");
    check_matches<int&>(R"(int\s*&)");
    check_matches<int const&>(R"(const\s+int\s*&|int\s+const\s*&)");
    check_matches<int(&)[]>(R"(int\s*\(\s*&\s*\)\s*\[\s*\])");
    check_matches<int(&)[10]>(R"(int\s*\(\s*&\s*\)\s*\[\s*10\s*\])");
    check_matches<Template<void, char const*>>(R"(Template<\s*void\s*,\s*(char const|const char)\s*\*\s*>)");
    check_matches<void(*)(int)>(R"(void\s*\(\s*\*\s*\)\s*\(\s*int\s*\))");
}


#ifdef _HANA_TN_CAN_CONSTEXPR
#define RUN_ONE_TYPE_TEST_COMPILE_TIME(type_definition, type_string_literal)                     \
        {                                                                                        \
            constexpr auto computed =                                                            \
                constype::internal::_impl_typeid_hana<type_definition>();                      \
            static_assert( boost::hana::experimental::type_name_details::stringliteral_equal_sz( \
                    computed, type_string_literal),                                              \
                "RUN_ONE_TYPE_TEST_COMPILE_TIME error");                                         \
        }
#else
#define RUN_ONE_TYPE_TEST_COMPILE_TIME(type_definition, type_string_literal)
#endif


void compile_time_tests() {
    RUN_ONE_TYPE_TEST_COMPILE_TIME(void, "constype::internal::TupleTypeHolder<void>");
    RUN_ONE_TYPE_TEST_COMPILE_TIME(char, "constype::internal::TupleTypeHolder<char>");

    // __PRETTY_FUNCTION__ seems to favor west-const (this is true for MSVC, GCC and Clang)
    // on the contrary, typeid().name() is strictly east const accross all compilers
    // (Does this really need to be tested ?)
    RUN_ONE_TYPE_TEST_COMPILE_TIME(const char, "constype::internal::TupleTypeHolder<const char>");
    RUN_ONE_TYPE_TEST_COMPILE_TIME(char const, "constype::internal::TupleTypeHolder<const char>");
}


/*
use with:

ninja | \
    gsed s/"', '"/""/g | \
    gsed s/"\\\\'"/"'"/g | \
    gsed -E s/"value of type 'boost::hana::string<'"/"\nHERE IS YOUR TYPE ==> "/g  | \
    grep "HERE IS YOUR TYPE" | \
    sed s/"'>' is not contextually convertible to 'bool'"//g | \
    sed s/"HERE IS YOUR TYPE ==> "//g
*/
#if 0
TEST_CASE("compile_type_typename")
{
    using T = std::vector<int>;
    constype::ERROR_full<T>();
}
#endif
