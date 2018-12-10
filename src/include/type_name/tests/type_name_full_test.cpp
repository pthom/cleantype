#include "doctest.h"
#include "type_name/type_name.hpp"

#define LOG(str) std::cout << str << std::endl
#define LOG_VALUE(var) std::cout << #var << " = " << var << std::endl

TEST_CASE("type_name_full_test")
{
    {
        // Standard types
        char v = 5;
        // The function type_name::full will add a reference
        // (since the arg is passed by universal reference (&&))
        REQUIRE_EQ(type_name::full(v),
        "char&"
        );
        // The macro will output the exact type
        REQUIRE_EQ(tn_type_name_full(v),
        "char"
        );
    }
    {
        // Reference
        char a = 5;
        char &v = a;
        REQUIRE_EQ(type_name::full(v),
        "char&"
        );
        REQUIRE_EQ(tn_type_name_full(v),
        "char&"
        );
    }
    {
        // Const reference
        char a = 5;
        const char &v = a;
        REQUIRE_EQ(type_name::full(v),
        "char const&"
        );
        REQUIRE_EQ(tn_type_name_full(v),
        "char const&"
        );
    }
    {
        // Pointer to const
#ifndef _MSC_VER // FIXME : msvc return "char const *&" (i.e with a space)
        char a = 5;
        const char *v = &a;
        REQUIRE_EQ(type_name::full(v),
        "char const*&"
        );
        REQUIRE_EQ(tn_type_name_full(v),
        "char const*"
        );
#endif
    }
    {
#ifndef _MSC_VER
        // Const pointer (but modifiable content)
        char a = 5;
        char * const v = &a;
        REQUIRE_EQ(type_name::full(v),
        "char* const&"
        );
        REQUIRE_EQ(tn_type_name_full(v),
        "char* const"
        );
#endif
    }
    {
        // Volatile
        volatile char v = 5;
        REQUIRE_EQ(type_name::full(v),
        "char volatile&"
        );
        REQUIRE_EQ(tn_type_name_full(v),
        "char volatile"
        );
    }
}


TEST_CASE("type_name_full_r_value_references")
{
    using TypenamePair = std::array<std::string, 2>;
    // r-value reference tests
    auto output_received_type = [](auto && v) -> TypenamePair {
        return { type_name::full<decltype(v)>(), type_name::full(v) };
    };
    auto require_eq_typename_pair = [](const TypenamePair & p1, const TypenamePair & p2 ) {
        REQUIRE_EQ(p1[0], p2[0]);
        REQUIRE_EQ(p1[1], p2[1]);
    };

    {
        // with a standard value
        char v = 5;
        require_eq_typename_pair(output_received_type(v),
        { "char&", "char&"}
        );
    }
    // with a reference
    {
        char a = 5;
        char &v = a;
        require_eq_typename_pair(output_received_type(v),
        { "char&", "char&"}
        );
    }
    // with a const reference
    {
        char a = 5;
        const char &v = a;
        require_eq_typename_pair(output_received_type(v),
        { "char const&", "char const&"}
        );
    }
    // with an r-value reference
    {
        std::string s("hello there, how are you. This is not a short string");
        require_eq_typename_pair(output_received_type(static_cast<char>(42)),
        { "char&&", "char&" }
        );
    }

}


TEST_CASE("tn_type_name_full")
{
    {
        char v = 5;
        REQUIRE_EQ(
            tn_type_name_full(v),
            "char"
         );
    }
    {
        REQUIRE_EQ(
            tn_type_name_full(static_cast<char>(42)),
            "char"
         );
    }
}


template<typename... Args>
struct TemplateClass
{
    static std::string full_type() {
        return type_name::full<Args...>();
    }
};


template<typename... Args>
void check_multiple_args(const std::string & expected)
{
    REQUIRE_EQ(
        type_name::full<Args...>(),
        expected
    );
    REQUIRE_EQ(
        TemplateClass<Args...>::full_type(),
        expected
    );
}


TEST_CASE("type_name_full_multiple")
{
    check_multiple_args<
         char, char const>(
        "char, char const");
    check_multiple_args<
         char&, char const& >(
        "char&, char const&"
    );
    check_multiple_args<
         char&&>(
        "char&&"
    );
    check_multiple_args<
         char&&>(
        "char&&"
    );

#ifndef _MSC_VER
    // FIXME : MSVC adds spaces "char *, char const *, char * const"
    check_multiple_args<
         char*, char const*, char* const >(
        "char*, char const*, char* const"
    );
#endif
}


template<typename... Args>
void check_multiple_args_fromvalues(const std::string & expected, Args... args)
{
    REQUIRE_EQ(
        type_name::full(args...),
        expected
    );
}


TEST_CASE("type_name_full_multiple_fromvalues")
{
    REQUIRE_EQ(
        type_name::full(static_cast<char>(1)),
        "char"
    );
    REQUIRE_EQ(
        type_name::full(static_cast<char>(1), static_cast<char>(1)),
        "char, char"
    );
#ifndef _MSC_VER
    REQUIRE_EQ(
        type_name::full(static_cast<char>(1), "hello"),
        "char, char [6] const&"
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
        char const& c = a;
        // REQUIRE_EQ(type_name::full(1, 2, c),
        // "char, char, char const&" --> "char, char, char" !!!
        // );
    }

}
