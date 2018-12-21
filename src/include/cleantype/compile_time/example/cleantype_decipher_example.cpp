#include <cleantype/details/fp_polyfill/fp_polyfill.hpp>
#include <string>
#include <map>
#include <vector>

// Intentionally bad code, in order to get the compiler to spit
// incomprehensible template errors

auto my_spurious_lambda = [](int a, int b)
{
    std::map<std::string, int> r;
    r[ fp::show(a) ] = b;
    return r;
};

auto my_spurious_lambda2 = [](auto v, int nb)
{
    std::vector<decltype(v)> r;
    for (std::size_t i = 0; i < nb; i++)
        r.push_back(v);
    return r;
};


auto my_spurious_lambda3 = [](const auto & v) {
    return v + 1;
};


int main()
{
    auto v = my_spurious_lambda(1, 2);
    auto v2 = my_spurious_lambda2(v, 3);

    auto v3 = fp::transform(my_spurious_lambda3, v2);
}
