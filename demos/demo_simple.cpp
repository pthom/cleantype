// Compile with
// cd demos/
// g++ -std=c++14 -Iinclude demo_simple.cpp -o demo_simple
//
// Edit and compile this file without installing anything at
// https://gitpod.io/#https://github.com/pthom/cleantype/blob/master/demos/demo_simple.cpp
//
#include <map>
#include <vector>
#include <cleantype/cleantype.hpp>

#define LOG(...) std::cout << __VA_ARGS__ << "\n";

void demo()
{
    // First, let's define a variable for demonstration purpose
    std::set<std::string> my_set { "Hello", "There"};

    // let's ask CleanType to give us the type of "my_set"
    // cleantype::full will return the *full* type info
    LOG(  cleantype::full(my_set)  );
    // outputs:
    // std::set<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>> &

    // Ouch, that was barely readable!
    // cleantype::clean will return a *readable* type
    LOG(  cleantype::clean(my_set) );
    // outputs:
    // std::set<std::string> &

    // Let's now show the content of "my_set" together with its type
    LOG(  cleantype::show_details(my_set) );
    // outputs:
    // std::set<std::string> & = [Hello, There]


    // Yes, but what about lambdas? Could you guess the signature of the lambda below?
    auto lambda_example = []() {
        // when C++ meets js...
        return +!!""; // See https://blog.knatten.org/2018/10/12/1662
    };

    // cleantype::lambda_clean returns the signature of lambda functions
    LOG(  cleantype::lambda_clean(lambda_example) );
    // outputs:
    // lambda: () -> int

    // Ok, maybe this was too easy. Let's try with a generic lambda!
    auto add = [](auto a, auto b) {
        return a + b;
    };

    // Now, can we see its signature?
    // Yes, we just need to specify the input argument types.
    LOG(  cleantype::lambda_clean<std::string, char>(add)  );
    // outputs:
    // lambda: (std::string, char) -> std::string
}


// Can CleanType understand some more complex libraries
// like range-v3 where most variables, functions and lambdas are of type "auto"?
// Well...    yes!

#include <range/v3/all.hpp>

using namespace ranges;

auto square_yield_fn(int x) {
    return ranges::yield(x * x);
}

void demo_ranges()
{
    auto squares_view = view::for_each(view::ints(1), square_yield_fn);

    // What is the type of squares_view?
    // Let's see...
    LOG( cleantype::clean(squares_view)  );
    // outputs:
    // ranges::v3::join_view<ranges::v3::transform_view<ranges::v3::iota_view<int, void>, ranges::v3::single_view<int>(*)(int)>, void> &

    // Let's make it more complex yet:
    auto squares_take_10 = squares_view | view::take(10);

    // As you will see below, CleanType can indent the types
    // when they get more complex!
    LOG(  cleantype::clean(squares_take_10)  );
    // outputs:
    // ranges::v3::detail::take_exactly_view_<
    //     ranges::v3::join_view<
    //         ranges::v3::transform_view<
    //             ranges::v3::iota_view<
    //                 int,
    //                 void
    //             >,
    //             ranges::v3::single_view<
    //                 int
    //             > (*)(int)
    //         >,
    //         void
    //     >,
    //     false
    // > &
}

int main()
{
    demo();
    demo_ranges();
}