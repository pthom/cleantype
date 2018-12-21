#include <cleantype/cleantype_compile_time.hpp>
#include <map>
#include <vector>
#include <string>

auto my_spurious_lambda = [](int a, int b)
{
    std::map<std::string, int> r;
    r[ fp::show(a) ] = b;
    return r;
};

// Usage
// WINDOWS : 
//            cmake --build . | bin\cleantype_compiler_parser.exe -c
// Clang:
//            cmake --build . | ./cleantype_compiler_parser.exe -c
int main()
{
    auto c = my_spurious_lambda(40, 2);
    TN_ERROR_full( decltype(c) );
    //cleantype::ERROR_full<decltype(c)>();
}
