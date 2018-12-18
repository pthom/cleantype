#include <type_name/details/hana_ext/experimental/detail/type_name_pretty_function.hpp>

#include <cstdlib>
#include <cstring>
#include <regex>

#include <iostream>

namespace cstring_utils = boost::hana::experimental::cstring_utils;


#ifdef _HANA_TN_CAN_CONSTEXPR
    #define RUN_ONE_TYPE_TEST_COMPILE_TIME(type_definition, type_string_literal)                 \
        {                                                                                        \
            constexpr auto computed =                                                            \
                cstring_utils::type_name_impl_cstring<type_definition>();                        \
            static_assert( cstring_utils::cstring_equal_literal(computed, type_string_literal),  \
                "STATIC_ASSERT_CSTRING_EQUAL error");                                            \
        }
#else
    #define RUN_ONE_TYPE_TEST_COMPILE_TIME(type_definition, type_string_literal)
#endif


template<typename... T>
struct Template {
};


template <typename T>
void check_matches(std::string const& re) {
    cstring_utils::cstring name_cs = cstring_utils::type_name_impl_cstring<T>();
    std::string name = cstring_utils::cstring_to_string(name_cs);
    std::regex regex{re};
    if (!std::regex_match(name, regex)) {
        std::cerr << "type name '" << name << "' does not match regex '" << re << "'" << std::endl;
        std::abort();
    }
}


void compile_time_tests() {
    RUN_ONE_TYPE_TEST_COMPILE_TIME(void, "void");
    RUN_ONE_TYPE_TEST_COMPILE_TIME(char, "char");

    // __PRETTY_FUNCTION__ seems to favor west-const (this is true for MSVC, GCC and Clang)
    // on the contrary, typeid().name() is strictly east const accross all compilers
    // (Does this really need to be tested ?)
    RUN_ONE_TYPE_TEST_COMPILE_TIME(const char, "const char");
    RUN_ONE_TYPE_TEST_COMPILE_TIME(char const, "const char");
}


void rumtime_regex_tests() {
    // Make sure we get something reasonable
    check_matches<int const>("int const|const int");
    check_matches<int&>(R"(int\s*&)");
    check_matches<int const&>(R"(const\s+int\s*&|int\s+const\s*&)");
    check_matches<int(&)[]>(R"(int\s*\(\s*&\s*\)\s*\[\s*\])");
    check_matches<int(&)[10]>(R"(int\s*\(\s*&\s*\)\s*\[\s*10\s*\])");
    check_matches<Template<void, char const*>>(R"(Template<\s*void\s*,\s*(char const|const char)\s*\*\s*>)");
    check_matches<void(*)(int)>(R"(void\s*\(\s*\*\s*\)\s*\(\s*int\s*\))");
}


int main() {
    compile_time_tests();
    rumtime_regex_tests();
    return 0;
}
