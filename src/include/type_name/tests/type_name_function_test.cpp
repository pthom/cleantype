#include "doctest.h"
#include <vector>
#include <type_name/type_name.hpp>

#if !defined(__clang__) && ( defined(__GNUC__) || defined(__GNUG__) )
    #define IS_GCC_COMPILER
#endif


auto my_make_range(int nb) // return std::vector<int>, but this is deduced
{
    std::vector<int> v;
    for (int i = 0; i < nb; i++)
        v.push_back(i);
    return v;
}

auto my_add(int a, int b)
{
    return a + b;
}

auto foo(int a, int b, int c, int d, int e) // return std::map<std::string, int>, but this is deduced
{
    return std::map<std::string, int> {
      { "1", a },
      { "2", b },
      { "3", c },
      { "4", d },
      { "5", e },
    };
}

#ifndef IS_GCC_COMPILER
TEST_CASE("log_wrap_function")
{
    REQUIRE_EQ(
      show_type_function_fromparams_1(my_make_range, 1),
      "function: (int) -> std::vector<int>"
    );
    REQUIRE_EQ(
      show_type_function_fromtypes_1(my_make_range, int),
      "function: (int) -> std::vector<int>"
    );

    // FIXME
    // REQUIRE_EQ(
    //   show_type_function_fromparams_2(my_make_range, 1, 1),
    //   "function: (int) -> std::vector<int>"
    // );

    // REQUIRE_EQ(
    //   show_type_function_fromparams_5(my_make_range, 1, 2, 3, 4, 5),
    //   "function: (int) -> std::vector<int>"
    // );
}
#endif // #ifndef IS_GCC_COMPILER
