// Special macro to change the include path (this is required for the binder demo)
#pragma cling add_include_path("../include")
#include <cleantype/cleantype.hpp>

#include <array>
#include <fplus/fplus.hpp>
#include <map>
#include <numeric>
#include <vector>

// This macro is used later in this manual in order to display commands and their results
#define run_show(...)                       \
    {                                       \
        std::cout << #__VA_ARGS__ << "\n";  \
        std::cout << __VA_ARGS__ << "\n\n"; \
    }

std::string int_to_roman(int value)
{
    // clang-format off
    std::vector<std::pair<int, char const*>> roman
    {
      { 1000, "M" },{ 900, "CM" },
      { 500, "D" },{ 400, "CD" },
      { 100, "C" },{ 90, "XC" },
      { 50, "L" },{ 40, "XL" },
      { 10, "X" },{ 9, "IX" },
      { 5, "V" },{ 4, "IV" },
      { 1, "I" }
    };
    // clang-format on
    std::string result;
    for (auto const & kv : roman)
    {
        while (value >= kv.first)
        {
            result += kv.second;
            value -= kv.first;
        }
    }
    return result;
}

std::string int_to_string(int value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}
