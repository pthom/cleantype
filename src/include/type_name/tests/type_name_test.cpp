#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "type_name/type_name_clean.hpp"

namespace type_name
{
// #define test_one_type(type_definition)      \
// type_definition xs;         \
// auto str_type_definition = var_type_name_full(xs); \
// /*std::cout << "Testing  #type_definition" */   \
// std::cout << "Full type: " << var_type_name_full(xs); \
// std::cout << "Clean type: " << type_name::demangle_typename(xs);

# define COMMA ,

template<typename T> void TestDefaultConstructibleType(const std::string & expectedTypeClean)
{
    const T v;
    std::string type_full = var_type_name_full(v);
    std::string type_clean = type_name::demangle_typename(type_full);
    std::cout << "type_full : " << type_full << "\n";
    std::cout << "type_clean: " << type_clean << '\n';
    std::cout << "expected  : " << expectedTypeClean << '\n';
    CHECK(type_clean == expectedTypeClean);
}


TEST_CASE("testing sample_lib")
{
    TestDefaultConstructibleType
        <
            const std::string
        >
        (
            "const std::string"
        );
    TestDefaultConstructibleType
        <
            const std::vector<int>
        >
        (
            "const std::vector<int>"
        );
    TestDefaultConstructibleType
        <
            const std::deque<unsigned long>
        >
        (
            "const std::deque<unsigned long>"
        );
    TestDefaultConstructibleType
        <
            const std::vector<std::pair<std::string, int>>
        >
        (
            "const std::vector<std::pair<std::string, int>>"
        );
    // {
    //     const std::vector<std::pair<std::string, int>> v;
    //     //const auto &&vv = std::move(v);
    //     std::string type_full = var_type_name_full(v);
    //     std::string type_clean = type_name::demangle_typename(type_full);
    //     std::cout << "type_full: " << type_full << "\n";
    //     std::cout << "type_clean: " << type_clean << '\n';
    // }
    // {
    //     std::cout << "\n";
    //     const std::vector<std::pair<std::string, int>> v;
    //     const auto &&vv = std::move(v);
    //     auto s = var_type_name_full(vv);
    //     std::cout << "Mangled:\n"
    //               << s << "\n";
    //     std::cout << "\n";
    //     std::cout << "Demangled:\n"
    //               << type_name::demangle_typename(s) << '\n';
    // }

    // {
    //     std::cout << "\n";
    //     auto f = [](const std::vector<std::string> &v, int a) {
    //         return v.size() + a;
    //     };
    //     auto s = var_type_name_full(f);
    //     std::cout << "Mangled:\n"
    //               << s << "\n";
    //     std::cout << "\n";
    //     std::cout << "Demangled:\n"
    //               << type_name::demangle_typename(s) << '\n';
    // }
}
}
