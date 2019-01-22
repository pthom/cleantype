#include <cleantype/details/hana_type_name/type_name_pretty_function.hpp>

#include <cstdlib>
#include <cstring>
#include <regex>

#include <iostream>

namespace type_name_details = boost::hana::experimental::type_name_details;

#ifdef _HANA_TN_CAN_CONSTEXPR
#define RUN_ONE_TYPE_TEST_COMPILE_TIME(type_definition, type_string_literal)                    \
    {                                                                                           \
        constexpr auto computed =                                                               \
            type_name_details::type_name_impl_stringliteral<type_definition>();                 \
        static_assert(type_name_details::stringliteral_equal_sz(computed, type_string_literal), \
                      "RUN_ONE_TYPE_TEST_COMPILE_TIME error");                                  \
    }
#else
#define RUN_ONE_TYPE_TEST_COMPILE_TIME(type_definition, type_string_literal)
#endif

template <typename... T>
struct Template
{
};

template <typename T>
void check_matches(std::string const & re)
{
    type_name_details::stringliteral name_cs = type_name_details::type_name_impl_stringliteral<T>();
    std::string name = type_name_details::stringliteral_to_string(name_cs);
    std::regex regex{re};
    if (!std::regex_match(name, regex))
    {
        std::cerr << "type name '" << name << "' does not match regex '" << re << "'" << std::endl;
        std::abort();
    }
}

void compile_time_tests()
{
    RUN_ONE_TYPE_TEST_COMPILE_TIME(void, "void");
    RUN_ONE_TYPE_TEST_COMPILE_TIME(char, "char");

    // __PRETTY_FUNCTION__ seems to favor west-const (this is true for MSVC, GCC and Clang)
    // on the contrary, typeid().name() is strictly east const accross all compilers
    // (Does this really need to be tested ?)
    RUN_ONE_TYPE_TEST_COMPILE_TIME(const char, "const char");
    RUN_ONE_TYPE_TEST_COMPILE_TIME(char const, "const char");
}

void rumtime_regex_tests()
{
    // Make sure we get something reasonable
    check_matches<int const>("int const|const int");
    check_matches<int &>(R"(int\s*&)");
    check_matches<int const &>(R"(const\s+int\s*&|int\s+const\s*&)");
    check_matches<int(&)[]>(R"(int\s*\(\s*&\s*\)\s*\[\s*\])");
    check_matches<int(&)[10]>(R"(int\s*\(\s*&\s*\)\s*\[\s*10\s*\])");
    check_matches<Template<void, char const *>>(
      R"((struct )?Template<\s*void\s*,\s*(char const|const char)\s*\*\s*>)");
    check_matches<void(*)(int)>(R"(void\s*\((__cdecl)?\s*\*\s*\)\s*\(\s*int\s*\))");
}

int main()
{
    compile_time_tests();
    rumtime_regex_tests();
    return 0;
}
