#include "doctest.h"
#include "constype/constype.hpp"
#include <fplus/fplus.hpp>
#include <functional>
#include <map>

#define LOG(str) std::cout << str << std::endl

TEST_CASE("TN_show_details")
{
  {
    int a = 1;
    REQUIRE_EQ(TN_show_details(a), "[int] a = 1");
  }
  {
    std::string s("hello");
    REQUIRE_EQ(TN_show_details(s), "[std::string] s = hello");
  }
  {
    std::vector<int> v{ { 1, 2, 3, 4, 5 } };
    REQUIRE_EQ(TN_show_details(v), "[std::vector<int>] v = [1, 2, 3, 4, 5]");
  }
  {
    std::map<std::string, int> v{ {
      { "a", 1 },
      { "b", 2 },
      { "c", 3 }
      } };
    auto t = TN_show_details_cont(v);
    REQUIRE_EQ(TN_show_details_cont(v), "[std::map<std::string, int>] v = [(a, 1), (b, 2), (c, 3)]");
  }
  {
    std::vector<std::string> v{ {
      { "a" },
      { "b" },
      { "c" }
      } };
    REQUIRE_EQ(TN_show_details(v), "[std::vector<std::string>] v = [a, b, c]");
  }
  {
    std::set<std::string> v{ {
      { "a" },
      { "b" },
      { "c" }
      } };
    REQUIRE_EQ(TN_show_details(v), "[std::set<std::string>] v = [a, b, c]");
  }
}


template<typename Transform>
auto make_test_string_transform(Transform f)
{
    return [f](const std::string & input, const std::string & expected_output) {
        std::string computed_output = f(input);
        REQUIRE_EQ(computed_output, expected_output);
    };
}



TEST_CASE("clean_typename_from_string")
{
    auto make_one_test = make_test_string_transform(constype::internal::impl_clean);
    make_one_test(
        "  int   ",
        "int");
    make_one_test(
        "int&",
        "int &");
    make_one_test(
        "int&&",
        "int &&");
    make_one_test(
        "std::pair<int,double> ",
        "std::pair<int, double>");
    make_one_test(
        "std::pair<int, double> ",
        "std::pair<int, double>");
    make_one_test(
        "std::pair < std::vector< std::string,int > >",
        "std::pair<std::vector<std::string, int>>");
    make_one_test(
        "std::pair < std::vector< std::string > >",
        "std::pair<std::vector<std::string>>");
    //make_one_test(
    //    "const int",
    //    "int const"); // compilers prefer east const and never emit west const type info !
    make_one_test(
        "int&",
        "int &");
    make_one_test(
        "std::string const&",
        "std::string const &");
    make_one_test(
        "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const &",
        "std::string const &");
    make_one_test(
        "class std::vector<int,class std::allocator<int> > const&",
        "std::vector<int> const &");
}


void compare_type_full_to_repr(const std::string & type_full, const std::string &expected_repr)
{
    std::string type_clean = constype::internal::impl_clean(type_full);
    std::string expected_repr2 = fp::replace_tokens(" COMMA ", ", ", expected_repr);
    REQUIRE_EQ(type_clean, expected_repr2);
}


template<typename T>
void impl_test_clean_type(const std::string & expectedRepr, T value)
{
    std::string type_full = TN_constype_full(value);
    compare_type_full_to_repr(type_full, expectedRepr);
}


#define test_clean_type__defaultcontructible(type_definition) \
    { \
    type_definition value = type_definition();  \
    impl_test_clean_type<type_definition>( #type_definition, value ); \
    impl_test_clean_type<type_definition const &>(#type_definition" const &", value); \
    }

#define COMMA ,

TEST_CASE("clean_typename_from_type")
{
    test_clean_type__defaultcontructible(int);
    test_clean_type__defaultcontructible(std::vector<int>);
    test_clean_type__defaultcontructible(std::pair<int COMMA int>);
    test_clean_type__defaultcontructible(std::deque<std::pair<std::string COMMA std::map<int COMMA int>>>);
    test_clean_type__defaultcontructible(std::set<int>);
    test_clean_type__defaultcontructible(std::list<std::string>);

    //{
   //    const std::vector<std::pair<std::string, int>> v;
   //    //const auto &&vv = std::move(v);
   //    std::string type_full = TN_constype_full(v);
   //    std::string type_clean = constype::clean(type_full);
   //    LOG(type_full);
   //    LOG(type_clean);
   //}
   //{
   //    std::cout << "\n";
   //    const std::vector<std::pair<std::string, int>> v;
   //    const auto &&vv = std::move(v);
   //    std::string type_full = TN_constype_full(vv);
   //    std::string type_clean = constype::clean(type_full);
   //}

   //{
   //    std::cout << "\n";
   //    auto f = [](const std::vector<std::string> &v, int a) {
   //        return v.size() + a;
   //    };
   //    std::string type_full = TN_constype_full(f);
   //    std::string type_clean = constype::clean(type_full);
   //}
}


TEST_CASE("clean_pack")
{
    REQUIRE_EQ(
         constype::clean<std::string>()
        ,                "std::string"
    );
    REQUIRE_EQ(
         constype::clean<std::string, std::vector<int>>()
        ,                "std::string, std::vector<int>"
    );
    REQUIRE_EQ(
         constype::clean<std::string, std::vector<int> const &, int const &>()
        ,                "std::string, std::vector<int> const &, int const &"
    );
}

TEST_CASE("clean_multiple_args")
{
    REQUIRE_EQ(
         constype::clean(std::string("ah"), 1)
        ,                "std::string, int"
    );
    std::vector<int> v;
    REQUIRE_EQ(
         constype::clean(std::string("ah"), &v, 1)
        ,                "std::string, std::vector<int> *, int"
    );
}

TEST_CASE("impl_clean")
{
    std::string typ_name = "std::__1::map<int, std::__1::vector<int, std::__1::allocator<int>>, std::__1::less<int>, std::__1::allocator<std::__1::pair<int const, std::__1::vector<int, std::__1::allocator<int>>>>>";
    std::string type_cleaned = constype::internal::impl_clean(typ_name);
    std::string expected = "std::map<int, std::vector<int>>";
    REQUIRE_EQ(type_cleaned, expected);
}