#include "doctest.h"
#include "type_name/type_name.hpp"
#include <fplus/fplus.hpp>
#include <functional>
#include <map>

#define LOG(str) std::cout << str << std::endl

TEST_CASE("show_details")
{
  {
    int a = 1;
    REQUIRE_EQ(show_details(a), "[int] a = 1");
  }
  {
    std::string s("hello");
    REQUIRE_EQ(show_details(s), "[std::string] s = hello");
  }
  {
    std::vector<int> v{ { 1, 2, 3, 4, 5 } };
    REQUIRE_EQ(show_details(v), "[std::vector<int>] v = [1, 2, 3, 4, 5]");
  }
  {
    std::map<std::string, int> v{ {
      { "a", 1 },
      { "b", 2 },
      { "c", 3 }
      } };
    auto t = show_details_cont(v);
#ifndef _MSC_VER // WIP !!!             "[std::map<std::string,int>] v = [(a, 1), (b, 2), (c, 3)]"
    REQUIRE_EQ(show_details_cont(v), "[std::map<std::string, int>] v = [(a, 1), (b, 2), (c, 3)]");
#endif
  }
  {
    std::vector<std::string> v{ {
      { "a" },
      { "b" },
      { "c" }
      } };
    REQUIRE_EQ(show_details(v), "[std::vector<std::string>] v = [a, b, c]");
  }
  {
    std::set<std::string> v{ {
      { "a" },
      { "b" },
      { "c" }
      } };
    REQUIRE_EQ(show_details(v), "[std::set<std::string>] v = [a, b, c]");
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

TEST_CASE("DISABLED_apply_east_const")
{
    //auto make_one_test = make_test_string_transform(type_name::DISABLED_apply_east_const);
    //make_one_test(
    //    "const std::string",
    //    "std::string const");
    //make_one_test(
    //    "const std::string&",
    //    "std::string const &");
    //make_one_test(
    //    "const std::string & ",
    //    "std::string const &");
}



TEST_CASE("clean_typename_from_string")
{
    auto make_one_test = make_test_string_transform(type_name::clean_typename);
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
    std::string type_clean = type_name::clean_typename(type_full);
    std::string expected_repr2 = fp::replace_tokens(" COMMA ", ", ", expected_repr);
    REQUIRE_EQ(type_clean, expected_repr2);
}


template<typename T>
void impl_test_clean_type(const std::string & expectedRepr, T value)
{
    std::string type_full = var_type_name_full(value);
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
   //    std::string type_full = var_type_name_full(v);
   //    std::string type_clean = type_name::clean_typename(type_full);
   //    LOG(type_full);
   //    LOG(type_clean);
   //}
   //{
   //    std::cout << "\n";
   //    const std::vector<std::pair<std::string, int>> v;
   //    const auto &&vv = std::move(v);
   //    std::string type_full = var_type_name_full(vv);
   //    std::string type_clean = type_name::clean_typename(type_full);
   //}

   //{
   //    std::cout << "\n";
   //    auto f = [](const std::vector<std::string> &v, int a) {
   //        return v.size() + a;
   //    };
   //    std::string type_full = var_type_name_full(f);
   //    std::string type_clean = type_name::clean_typename(type_full);
   //}
}
