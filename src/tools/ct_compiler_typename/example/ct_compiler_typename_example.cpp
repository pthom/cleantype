#include <cleantype/cleantype_compiler_typename.hpp>
#include <cleantype/details/fp_polyfill/fp_polyfill.hpp>
#include <map>
#include <vector>
#include <string>

auto my_spurious_lambda = [](int a, int b)
{
    std::map<std::string, int> r;
    r[ fp::show(a) ] = b;
    return r;
};

template<typename... T>
void ShowType()
{
#ifdef _MSC_VER
    static_assert(false, "Here is the type");
#else
    static_assert(T() +1 == std::string("42"), "Here is the type");
#endif
}

#define CompilerLogVarType(var) ShowType<decltype(var)>();
#define CompilerLogType(T) ShowType<T>();

// Usage
// WINDOWS :
//            cmake --build . | bin\cleantype_compiler_parser.exe -c
// Clang:
//            cmake --build . | ./cleantype_compiler_parser.exe -c
int main()
{
    auto c = my_spurious_lambda(40, 2);
    //TN_ERROR_full_var(c);
    //cleantype::ERROR_full<decltype(c)>();

    // MSVC
    {
        // CompilerLogVarType(c); // OK
        // ShowType<decltype(c)>(); // OK
        // CompilerLogType(decltype(c)); // OK
    }

    auto s = BOOST_HANA_STRING("hello");

    std::map<int, std::vector<std::string>> v;
    //s++;
    //RefuseAlways<decltype(s)>();
    //CompilerLogVarType(s);
    //CompilerLogVarType(v);


}


/*

Clang example session:

> ninja| ./cleantype_compiler_parser -c
std::map<std::string, int>
	at: ../src/include/cleantype/compile_time/example/cleantype_compiler_typename_example.cpp:21:5:
    TN_ERROR_full_var(c);
    ^
	Compiler line #4
MSVC example session (in a msdos console, not a git bash console):

> cmake --build . | bin\cleantype_compiler_parser.exe -c
std::map<std::string, int>
        at:   F:\dvp\OpenSource\type_name\src\include\cleantype\compile_time\example\cleantype_compiler_typename_example.cpp(16)
        Compiler line #196
*/