#include "doctest.h"
#include "type_name/type_name.hpp"
#include <fplus/fplus.hpp>
#include <functional>
#include <map>

#define LOG(str) std::cout << str << std::endl

TEST_CASE("log_var")
{
  {
    int a = 1;
    REQUIRE_EQ(log_var_str(a), "[int] a = 1");
  }
  {
    std::string s("hello");
    REQUIRE_EQ(log_var_str(s), "[std::string] s = hello");
  }
  {
    std::vector<int> v{ { 1, 2, 3, 4, 5 } };
    REQUIRE_EQ(log_var_str(v), "[std::vector<int>] v = [1, 2, 3, 4, 5]");
  }
  {
    std::map<std::string, int> v{ {
      { "a", 1 },
      { "b", 2 },
      { "c", 3 }
      } };
    auto t = log_var_str_cont(v);
#ifndef _MSC_VER // WIP !!!             "[std::map<std::string,int>] v = [(a, 1), (b, 2), (c, 3)]"
    REQUIRE_EQ(log_var_str_cont(v), "[std::map<std::string, int>] v = [(a, 1), (b, 2), (c, 3)]");
#endif
  }
  {
    std::vector<std::string> v{ {
      { "a" },
      { "b" },
      { "c" }
      } };
    REQUIRE_EQ(log_var_str(v), "[std::vector<std::string>] v = [a, b, c]");
  }
  {
    std::set<std::string> v{ {
      { "a" },
      { "b" },
      { "c" }
      } };
    REQUIRE_EQ(log_var_str(v), "[std::set<std::string>] v = [a, b, c]");
  }
}



TEST_CASE("demangle_test_fixme")
{
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
