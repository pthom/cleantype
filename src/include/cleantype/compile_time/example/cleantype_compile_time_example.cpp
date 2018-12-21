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


/*

Clang example session:

> ninja 2>&1 | ./cleantype_compiler_parser -c
std::map<std::string, int>
	at: ../src/include/cleantype/compile_time/example/cleantype_compile_time_example.cpp:21:5:
    TN_ERROR_full( decltype(c) );
    ^
	Compiler line #4

MSVC example session (in a msdos console, not a git bash console):

> cmake --build . 2>&1 | bin\cleantype_compiler_parser.exe -c
std::map<std::string, int>
        at:   F:\dvp\OpenSource\type_name\src\include\cleantype\compile_time\example\cleantype_compile_time_example.cpp(16)
        Compiler line #188

F:\dvp\OpenSource\type_name\build> cmake --build . | bin\cleantype_compiler_parser.exe -c
std::map<std::string, int>
        at:   F:\dvp\OpenSource\type_name\src\include\cleantype\compile_time\example\cleantype_compile_time_example.cpp(16)
        Compiler line #188
*/