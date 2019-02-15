
// This demo showcases how ct_compiler_decipher can
// make the compiler output more readable.
//
// 1. First compile ct_compiler_decipher:
// > g++ -Iinclude -std=c++14 ../src/tools/ct_compiler_decipher/ct_compiler_decipher.cpp -o ct_compiler_decipher
//
// 2. Try to compile this file (it will fail, this is intentional)
// > g++ -std=c++14 -Iinclude demo_compiler_decipher.cpp 2>&1 | less
// The error will resemble this:
//        demo_compiler_decipher.cpp:21:23: error: no match for ‘operator+’
//        (operand types are ‘const std::set<std::__cxx11::basic_string<char> >’ and ‘int’)
//
// 3. Compile this file, and pipe the compiler output to ct_compiler_decipher
// > g++ -std=c++14 -Iinclude demo_compiler_decipher.cpp 2>&1 | ./ct_compiler_decipher | less
// The error will resemble this:
//        demo_compiler_decipher.cpp:21:23: error: no match for ‘operator+’
//        (operand types are ‘const std::set<std::string> ’ and ‘int’)

#include <algorithm>
#include <set>
#include <vector>
#include <map>
#include <string>

int main() {
    std::vector<std::set<std::string>> v;
    std::transform(
        v.begin(), v.end(),
        v.begin(),
        [](const auto & xs) {
            return xs + 1; // ouch, error here!
        }
    );
}
