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

    {
        std::string s("std::deque<std::pair<std::string, std::map<int, int>>>");
        cleantype::CleanConfiguration::GlobalConfig().indent_depth_limit = 3;
        std::string expected("std::deque<std::pair<std::string, std::map<int, int>>>");
        auto indented = cleantype::internal::impl_indent_if_neeeded(s);
        REQUIRE_EQ(indented, expected);
    }
    {
        std::string s("std::deque<std::pair<std::string, std::map<int, int>>>");
        std::size_t old_limit = cleantype::CleanConfiguration::GlobalConfig().indent_depth_limit;
        cleantype::CleanConfiguration::GlobalConfig().indent_depth_limit = 2;
        std::string expected = R"(std::deque
<
    std::pair
    <
        std::string,
        std::map
        <
            int,
            int
        >
    >
>)";
        auto indented = cleantype::internal::impl_indent_if_neeeded(s);
        REQUIRE_EQ(indented, expected);
        cleantype::CleanConfiguration::GlobalConfig().indent_depth_limit = old_limit;
    }
}
