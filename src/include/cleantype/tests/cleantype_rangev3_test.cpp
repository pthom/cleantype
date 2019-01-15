#include "doctest.h"
#include <array>
#include <iostream>
#include <range/v3/algorithm/count_if.hpp> // specific includes
#include <vector>
#include <cleantype/cleantype.hpp>
using std::cout;

#define LOG(...) std::cout << __VA_ARGS__ << "\n";
#define LOG_VAR(...) std::cout << #__VA_ARGS__ << "\n" << __VA_ARGS__ << "\n";

// example/count_if.cpp

auto is_six = [](int i) -> bool { return i == 6; };

// third_party/tests/range-v3/example/comprehensions.cpp
#include <chrono>
#include <iostream>
#include <range/v3/all.hpp>


TEST_CASE("range-v3-cleantype")
{
    using namespace ranges;

    auto square_yield = [](int x) {
        return yield(x * x);
    };
    LOG_VAR(cleantype::lambda_clean(square_yield)); // lambda: (int) -> ranges::v3::single_view<int>

    auto squares_view = view::for_each(view::ints(1), square_yield);
    LOG_VAR(cleantype::clean(squares_view));
    // cleantype::clean(squares_view) ==>
    //     ranges::v3::join_view
    //     <
    //         ranges::v3::transform_view
    //         <
    //             ranges::v3::iota_view<int, void>,
    //             (lambda at ../src/include/cleantype/tests/cleantype_rangev3_test.cpp:103:50)
    //         >,
    //         void
    //     > &

    // cleantype::clean(squares2) ==>
    //     ranges::v3::join_view
    //     <
    //         ranges::v3::transform_view
    //         <
    //             ranges::v3::iota_view<int, void>,
    //             lambda: (int) -> ranges::v3::single_view<int>
    //         >,
    //         void
    //     > &


    auto square_take_10 = squares_view | view::take(10);
    LOG_VAR(cleantype::clean(square_take_10));
    // ranges::v3::detail::take_exactly_view_
    // <
    //     ranges::v3::join_view
    //     <
    //         ranges::v3::transform_view
    //         <
    //             ranges::v3::iota_view
    //             <
    //                 int,
    //                 void
    //             >,
    //             (lambda at ../src/include/cleantype/tests/cleantype_rangev3_test.cpp:97:25)
    //         >,
    //         void
    //     >,
    //     false
    // > &

    //triples | view::take(100)
}
