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


TEST_CASE("testing sample_lib")
{
    {
        std::cout << "\n";
        const std::vector<std::pair<std::string, int>> v;
        const auto &&vv = std::move(v);
        auto s = var_type_name_full(vv);
        std::cout << "Full type:" << s << "\n";
        std::cout << "Clean Type:" << type_name::demangle_typename(s) << '\n';
    }
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
