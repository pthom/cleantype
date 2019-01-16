#include <fplus/fplus.hpp>
#include <map>
#include <string>
#include <vector>

// Intentionally bad code, in order to get the compiler to spit
// incomprehensible template errors

auto my_spurious_lambda = [](int a, int b) {
    std::map<std::string, int> r1;
    std::vector<decltype(r1)> r2;
    for (std::size_t i = 0; i < b; i++)
        r2.push_back(r1);
    auto add_one = [](auto x) { return x + 1; };
    auto r3 = fplus::transform(add_one, r2);
    return r3;
};

int main() { auto v = my_spurious_lambda(1, 3); }
