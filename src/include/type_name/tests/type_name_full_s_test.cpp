#include "doctest.h"
#include "type_name/details/type_name_full_s.hpp"

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
    //type_name_s::ERROR_full<int>();
}

TEST_CASE("type_name_full_test_s")
{
    {
        // Standard types
        char v = 5;
        MY_REQUIRE_EQ_STRING(type_name_s::full(v),
        "char"
        );
        // The macro will output the exact type
        MY_REQUIRE_EQ_STRING(TNS_type_name_full(v),
        "char"
        );
    }
    {
        // Reference
        char a = 5;
        char &v = a;
        MY_REQUIRE_EQ_STRING(type_name_s::full(v),
        "char"
        );
        MY_REQUIRE_EQ_STRING(TNS_type_name_full(v),
        "char &"
        );
    }
    {
        // Const reference
        char a = 5;
        const char &v = a;
        MY_REQUIRE_EQ_STRING(type_name_s::full(v),
        "char"
        );
        MY_REQUIRE_EQ_STRING(TNS_type_name_full(v),
        "const char &"
        );
    }
    {
        // Pointer to const
 // FIXME : msvc return "const char *&" (i.e with a space)
        char a = 5;
        const char *v = &a;
        MY_REQUIRE_EQ_STRING(type_name_s::full(v),
        "const char *"
        );
        MY_REQUIRE_EQ_STRING(TNS_type_name_full(v),
        "const char *"
        );
    }
    {
        // Const pointer (but modifiable content)
        char a = 5;
        char * const v = &a;
        MY_REQUIRE_EQ_STRING(type_name_s::full(v),
        "char *"
        );
        MY_REQUIRE_EQ_STRING(TNS_type_name_full(v),
        "char * const"
        );
    }
    {
        // Volatile
        volatile char v = 5;
        MY_REQUIRE_EQ_STRING(type_name_s::full(v),
        "char"
        );
        MY_REQUIRE_EQ_STRING(TNS_type_name_full(v),
        "volatile char"
        );
    }
}


TEST_CASE("type_name_full_r_value_references")
{
    using TypenamePair = std::array<std::string, 2>;
    // r-value reference tests
    auto output_received_type = [](auto && v) -> TypenamePair {
        return { type_name_s::full<decltype(v)>(), type_name_s::full(v) };
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
        { "char &", "char"}
        );
    }
    // with a reference
    {
        char a = 5;
        char &v = a;
        require_eq_typename_pair(output_received_type(v),
        { "char &", "char"}
        );
    }
    // with a const reference
    {
        char a = 5;
        const char &v = a;
        require_eq_typename_pair(output_received_type(v),
        { "const char &", "char"}
        );
    }
    // with an r-value reference
    {
        std::string s("hello there, how are you. This is not a short string");
        require_eq_typename_pair(output_received_type(static_cast<char>(42)),
        { "char &&", "char" }
        );
    }

}


 TEST_CASE("TNS_type_name_full")
 {
     {
         char v = 5;
         MY_REQUIRE_EQ_STRING(
             TNS_type_name_full(v),
             "char"
          );
     }
     {
         MY_REQUIRE_EQ_STRING(
             TNS_type_name_full(static_cast<char>(42)),
             "char"
          );
     }
 }



 template<typename... Args>
 struct TemplateClass
 {
     static std::string full_type() {
         return type_name_s::full<Args...>();
     }
 };


 template<typename... Args>
 void check_multiple_args(const std::string & expected)
 {
     auto v1 = type_name_s::full<Args...>();
     auto v2 = TemplateClass<Args...>::full_type();
     if (v1 != expected)
         std::cout << "pb1\n";
     if (v2 != expected)
         std::cout << "pb2\n";
     MY_REQUIRE_EQ_STRING(
         type_name_s::full<Args...>(),
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
         type_name_s::full(args...),
         expected
     );
 }


 TEST_CASE("type_name_full_multiple_fromvalues")
 {
     MY_REQUIRE_EQ_STRING(
         type_name_s::full(static_cast<char>(1)),
         "char"
     );
     MY_REQUIRE_EQ_STRING(
         type_name_s::full(static_cast<char>(1), static_cast<char>(1)),
         "char, char"
     );
     std::string tst = type_name_s::full(static_cast<char>(1), "hello");
 #ifndef _MSC_VER // msvc emits "char, const char[6] const &"
     MY_REQUIRE_EQ_STRING(
         type_name_s::full(static_cast<char>(1), "hello"),
         "char, const char *"
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
          MY_REQUIRE_EQ_STRING(type_name_s::full('0', '0', c),
          "char, char, char" //--> "char, char, char" !!!
          );
     }

 }
