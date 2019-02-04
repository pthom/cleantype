#include <cleantype/cleantype.hpp>

#include <map>
#include <vector>

#define LOG(...) std::cout << __VA_ARGS__ << "\n";

void demo()
{
    std::vector<int> v{1, 2, 3};
    LOG(CT_show_details(v));  // outputs: [std::vector<int>] v = [1, 2, 3]

    std::map<std::string, int> v2{{"val1", 1}, {"val2", 2}};
    LOG(CT_show_details(v2));  // outputs: [std::map<std::string, int>] v2 = [(val1, 1), (val2, 2)]

    cleantype::set_indent_depth_limit(2);
    std::deque<std::pair<std::string, std::map<int, int>>> v3 = {{"Hello", {{2, 3}}}};
    // The line below will output an indented type (see the prior call to set_indent_depth_limit)
    // outputs:
    // [std::deque<
    //     std::pair<
    //         std::string,
    //         std::map<
    //             int,
    //             int
    //         >
    //     >
    // >] v3 = [(Hello, [(2, 3)])]
    LOG(CT_show_details(v3));

    // lambda can be outputed also
    auto my_add = [](int a, int b) { return a + b; };
    LOG(CT_show_details_lambda(my_add));  // outputs: [lambda: (int, int) -> int] my_add
}

int main() { demo(); }