#include "doctest.h"
#include "type_name/type_name.hpp"

#define LOG(str) std::cout << str << std::endl
#define LOG_VALUE(var) std::cout << #var << " = " << var << std::endl

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

TEST_CASE("type_name_full_test")
{
    {
        // Standard types
        char v = 5;
        // The function type_name::full will add a reference
        // (since the arg is passed by universal reference (&&))
        MY_REQUIRE_EQ_STRING(type_name::full(v),
        "char &"
        );
        // The macro will output the exact type
        MY_REQUIRE_EQ_STRING(TN_type_name_full(v),
        "char"
        );
    }
    {
        // Reference
        char a = 5;
        char &v = a;
        MY_REQUIRE_EQ_STRING(type_name::full(v),
        "char &"
        );
        MY_REQUIRE_EQ_STRING(TN_type_name_full(v),
        "char &"
        );
    }
    {
        // Const reference
        char a = 5;
        const char &v = a;
        MY_REQUIRE_EQ_STRING(type_name::full(v),
        "char const &"
        );
        MY_REQUIRE_EQ_STRING(TN_type_name_full(v),
        "char const &"
        );
    }
    {
        // Pointer to const
 // FIXME : msvc return "char const *&" (i.e with a space)
        char a = 5;
        const char *v = &a;
        MY_REQUIRE_EQ_STRING(type_name::full(v),
        "char const * &"
        );
        MY_REQUIRE_EQ_STRING(TN_type_name_full(v),
        "char const *"
        );
    }
    {
        // Const pointer (but modifiable content)
        char a = 5;
        char * const v = &a;
        MY_REQUIRE_EQ_STRING(type_name::full(v),
        "char * const &"
        );
        MY_REQUIRE_EQ_STRING(TN_type_name_full(v),
        "char * const"
        );
    }
    {
        // Volatile
        volatile char v = 5;
        MY_REQUIRE_EQ_STRING(type_name::full(v),
        "char volatile &"
        );
        MY_REQUIRE_EQ_STRING(TN_type_name_full(v),
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
        { "char const &", "char const &"}
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


TEST_CASE("TN_type_name_full")
{
    {
        char v = 5;
        MY_REQUIRE_EQ_STRING(
            TN_type_name_full(v),
            "char"
         );
    }
    {
        MY_REQUIRE_EQ_STRING(
            TN_type_name_full(static_cast<char>(42)),
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
    MY_REQUIRE_EQ_STRING(
        type_name::full<Args...>(),
        expected
    );
    MY_REQUIRE_EQ_STRING(
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
         char &, char const & >(
        "char &, char const &"
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
         char *, char const *, char * const >(
        "char *, char const *, char * const"
    );
}


template<typename... Args>
void check_multiple_args_fromvalues(const std::string & expected, Args... args)
{
    MY_REQUIRE_EQ_STRING(
        type_name::full(args...),
        expected
    );
}


TEST_CASE("type_name_full_multiple_fromvalues")
{
    MY_REQUIRE_EQ_STRING(
        type_name::full(static_cast<char>(1)),
        "char"
    );
    MY_REQUIRE_EQ_STRING(
        type_name::full(static_cast<char>(1), static_cast<char>(1)),
        "char, char"
    );
    std::string tst = type_name::full(static_cast<char>(1), "hello");
#ifndef _MSC_VER // msvc emits "char, char const[6] const &"
    MY_REQUIRE_EQ_STRING(
        type_name::full(static_cast<char>(1), "hello"),
        "char, char[6] const &"
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
        // MY_REQUIRE_EQ_STRING(type_name::full(1, 2, c),
        // "char, char, char const&" --> "char, char, char" !!!
        // );
    }

}
