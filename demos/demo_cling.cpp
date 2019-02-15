// This is the source code for the main "video" (asciinema) demo
// The video was created using the following commands:
// > asciinema rec demo_cling.cast
// > python im_not_a_touch_typist.py demo_cling.cpp | tee /dev/tty | cling --std=c++14
// > Ctrl-D
#pragma cling add_include_path("./src/include")
#pragma cling add_include_path("third_party/tests_thirdparty/FunctionalPlus/include")
#pragma cling add_include_path("third_party/tests_thirdparty/range-v3/include")
#include <numeric>


// CleanType (https://github.com/pthom/cleantype)
// Readable C++ type introspection

// This demo uses cling, a fully compliant C++14 REPL, and asciinema, a terminal session recorder.
// You can pause at any time, and copy-paste samples from it.

#include <cleantype/cleantype.hpp>

// The dumbest logger in the west...
#define LOG(...) std::cout << __VA_ARGS__ << "\n";

// First, let's define a variable for demonstration purpose
std::set<std::string> my_set { "Hello", "There"};

// let's ask CleanType to give us the type of "my_set"
// cleantype::full will return the *full* type info
LOG(  cleantype::full(my_set)  );


// Ouch, that was barely readable!
// cleantype::clean will return a *readable* type
LOG(  cleantype::clean(my_set) );

// Let's now show the content of "my_set" together with its type
LOG(  cleantype::show_details(my_set) );


// Yes, but what about lambdas? Could you guess the signature of the lambda below?
auto lambda_example = []() {
    // when C++ meets js...
    return +!!""; // See https://blog.knatten.org/2018/10/12/1662
};

// cleantype::lambda_clean returns the signature of lambda functions
LOG(  cleantype::lambda_clean(lambda_example) );



// Ok, maybe this was too easy. Let's try with a generic lambda!
auto add = [](auto a, auto b) {
    return a + b;
};

// Now, can we see its signature?
// Yes, we just need to specify the input argument types.
LOG(  cleantype::lambda_clean<std::string, char>(add)  );







// Can CleanType understand some more complex libraries
// like range-v3 where most variables, functions and lambdas are of type "auto"?
// Well...    yes!

#include <range/v3/all.hpp>

using namespace ranges;

auto square_yield_fn(int x) {
  return ranges::yield(x * x);
}
auto squares_view = view::for_each(view::ints(1), square_yield_fn);

// What is the type of squares_view?
// Let's see...
LOG( cleantype::clean(squares_view)  );

// Let's make it more complex yet:
auto squares_take_10 = squares_view | view::take(10);

// As you will see below, CleanType can indent the types
// when they get more complex!

LOG(  cleantype::clean(squares_take_10)  );



// Thanks for watching!





.q