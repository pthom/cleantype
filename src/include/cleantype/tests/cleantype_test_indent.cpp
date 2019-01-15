#include "doctest.h"
#include <cleantype/cleantype.hpp>

#define LOG(...) std::cout << __VA_ARGS__ << "\n";
#define LOG_VAR(...) std::cout << #__VA_ARGS__ << "\n" << __VA_ARGS__ << "\n";

template<typename... T> struct Foo {};

TEST_CASE("indent")
{
    {
        std::string s("int");
        auto indented = cleantype::indent_type_tree(s);
        std::string expected = "int";
        REQUIRE_EQ(indented, expected);
    }

    {
        std::string s("Foo<int>");
        auto indented = cleantype::indent_type_tree(s);
        LOG_VAR(indented);
        std::string expected = R"(Foo
<
    int
>)";
        REQUIRE_EQ(indented, expected);
    }

    {
        std::string s("Foo<int, char>");
        auto indented = cleantype::indent_type_tree(s);
        std::string expected = R"(Foo
<
    int,
    char
>)";
        REQUIRE_EQ(indented, expected);
    }

    {
        std::string s("Foo< int, Foo<int, Foo<std::string, int>>>");
        auto indented = cleantype::indent_type_tree(s);
        std::string expected = R"(Foo
<
    int,
    Foo
    <
        int,
        Foo
        <
            std::string,
            int
        >
    >
>)";
        REQUIRE_EQ(indented, expected);
    }
}
