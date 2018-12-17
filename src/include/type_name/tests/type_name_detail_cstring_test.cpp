#include "doctest.h"
#include <type_name/details/hana_utils/experimental/detail/type_name_pretty_function.hpp>
#include <type_name/details/hana_utils/experimental/detail/type_name_format.hpp>

#include <cstdlib>
#include <cstring>

#include <iostream>

namespace hana = boost::hana;
namespace cstring_utils = boost::hana::experimental::cstring_utils;


void assert_types_def_equal_runtime(const std::string & computed, const std::string & expected) {
    bool are_equal = (computed == expected);
    if (! are_equal) {
        std::cerr
            << "Error in assert_cstring_equal_runtime :\n"
            << "\tComputed:\n\t\t"
            << computed << "\n"
            << "\tExpected:\n\t\t"
            << expected << "\n";
    }
    REQUIRE(are_equal);
}


#ifdef _HANA_TN_CAN_CONSTEXPR
    #define RUN_ONE_TYPE_TEST_COMPILE_TIME(type_definition, type_string_literal)                 \
        {                                                                                        \
            constexpr auto computed =                                                            \
                hana::experimental::cstring_utils::type_name_impl_cstring<type_definition>();    \
            static_assert( cstring_utils::cstring_equal_literal(computed, type_string_literal),  \
                "STATIC_ASSERT_CSTRING_EQUAL error");                                            \
        }
#else
    #define RUN_ONE_TYPE_TEST_COMPILE_TIME(type_definition, type_string_literal)
#endif

template<typename TypeDefinition>
void run_one_type_test_run_time(const std::string & type_string_literal) {
    auto computed_type_definition =
        hana::experimental::cstring_utils::type_name_impl_cstring<TypeDefinition>();
    const std::string as_string = cstring_utils::cstring_to_string(computed_type_definition);
    const std::string formatted_type = hana::experimental::type_name_format::format_type(as_string);
    assert_types_def_equal_runtime(formatted_type, type_string_literal);
}

#define RUN_ONE_TYPE_TEST_RUN_TIME(type_definition, type_string_literal)                 \
    run_one_type_test_run_time<type_definition>(type_string_literal)

#define COMMA ,

template<typename... T>
struct Template {
};

// template <typename T>
// void check_matches(std::string const& re) {
//     std::string name = hana::to<char const*>(hana::experimental::type_name<T>());
//     std::regex regex{re};
//     if (!std::regex_match(name, regex)) {
//         std::cerr << "type name '" << name << "' does not match regex '" << re << "'" << std::endl;
//         std::abort();
//     }
// }


//////////// Actual tests Below

// void original_test()
// {
//     // Make sure this can be obtained at compile-time
//     BOOST_HANA_CONSTANT_CHECK(hana::equal(
//         hana::experimental::type_name<void>(),
//         BOOST_HANA_STRING("void")
//     ));

//     BOOST_HANA_CONSTANT_CHECK(hana::equal(
//         hana::experimental::type_name<int>(),
//         BOOST_HANA_STRING("int")
//     ));

//     // Make sure we get something reasonable
//     check_matches<int const>("int const|const int");
//     check_matches<int&>(R"(int\s*&)");
//     check_matches<int const&>(R"(const\s+int\s*&|int\s+const\s*&)");
//     check_matches<int(&)[]>(R"(int\s*\(\s*&\s*\)\s*\[\s*\])");
//     check_matches<int(&)[10]>(R"(int\s*\(\s*&\s*\)\s*\[\s*10\s*\])");
//     check_matches<Template<void, char const*>>(R"(Template<\s*void\s*,\s*(char const|const char)\s*\*\s*>)");
//     check_matches<void(*)(int)>(R"(void\s*\(\s*\*\s*\)\s*\(\s*int\s*\))");
// }

TEST_CASE("type_name_detail_cstring_test_compile_time") {
    RUN_ONE_TYPE_TEST_COMPILE_TIME(void, "void");
    RUN_ONE_TYPE_TEST_COMPILE_TIME(char, "char");


    // const tests : __PRETTY_FUNCTION__ seems to favor west-const
    // although, this behavior is not very consistent. This is true for MSVC, gcc and clang.
    // Note : on the contrary, typeid().name() is strictly east const accross compilers
    RUN_ONE_TYPE_TEST_COMPILE_TIME(const char, "const char");
    RUN_ONE_TYPE_TEST_COMPILE_TIME(char const, "const char");

    // Test below cannot be run at compile time, because of different formatting for "*" and "&"
    // MSVC does not add space before * or &, gcc and clang do
    //RUN_ONE_TYPE_TEST_COMPILE_TIME(char &, "char &");
    //RUN_ONE_TYPE_TEST_COMPILE_TIME(char * const, "char * const");
    //RUN_ONE_TYPE_TEST_COMPILE_TIME(char &&, "char &&");
    //RUN_ONE_TYPE_TEST_COMPILE_TIME(char *, "char *");
    //RUN_ONE_TYPE_TEST_COMPILE_TIME(Template<char COMMA int>, "Template<char, int>");
    //RUN_ONE_TYPE_TEST_COMPILE_TIME(const char &, "const char &");
    //RUN_ONE_TYPE_TEST_COMPILE_TIME(const char *, "const char *");

#ifndef _MSC_VER
    RUN_ONE_TYPE_TEST_RUN_TIME(Template<char>, "Template<char>");
#else
    RUN_ONE_TYPE_TEST_RUN_TIME(Template<char>, "struct Template<char>");
#endif
}

TEST_CASE("type_name_detail_cstring_test_run_time") {
    // Those test might fail at compile time because of different type formatting across compilers
    // (spaces before/after *, etc.)
    RUN_ONE_TYPE_TEST_RUN_TIME(char * const, "char * const");
    RUN_ONE_TYPE_TEST_RUN_TIME(char const *, "const char *");
}


/*
    // Make sure we get something reasonable
    check_matches<int const>("int const|const int");
    check_matches<int&>(R"(int\s*&)");
    check_matches<int const&>(R"(const\s+int\s*&|int\s+const\s*&)");
    check_matches<int(&)[]>(R"(int\s*\(\s*&\s*\)\s*\[\s*\])");
    check_matches<int(&)[10]>(R"(int\s*\(\s*&\s*\)\s*\[\s*10\s*\])");
    check_matches<Template<void, char const*>>(R"(Template<\s*void\s*,\s*(char const|const char)\s*\*\s*>)");
    check_matches<void(*)(int)>(R"(void\s*\(\s*\*\s*\)\s*\(\s*int\s*\))");
*/