#pragma once

#include <boost/hana.hpp>
#include <boost/hana/reverse.hpp>
//#include <boost/hana/fwd/reverse.hpp>
#include <boost/hana/fwd/drop_while.hpp>
//#include <boost/hana/for_each.hpp>

namespace hana_tn_utils {


auto reverse_boost_hana_string = [](auto s) {
    auto add_reverse = [=](auto xs, auto ys) {
        auto ys_s = boost::hana::string<ys>();
        return ys_s + xs;
    };
    auto reversed = boost::hana::fold_left(s, BOOST_HANA_STRING(""), add_reverse);
    return reversed;
};


auto trim_string_left_c = [](auto trim_what, auto xs) {
    auto should_trim = [&trim_what](auto x) {
        return x == trim_what;
    };
    auto ys = boost::hana::drop_while(xs, should_trim);
    return ys;
};


auto trim_string_right_c = [](auto trim_what, auto xs) {
    auto xs_r = reverse_boost_hana_string(xs);
    auto xs_r_trim = trim_string_left_c(trim_what, xs_r);
    auto trimmed = reverse_boost_hana_string(xs_r_trim);
    return trimmed;
};


auto trim_string_c = [](auto trim_what, auto xs) {
    auto x1 = trim_string_left_c(trim_what, xs);
    auto x2 = trim_string_right_c(trim_what, x1);
    return x2;
};


auto trim_string_left_space = [](auto xs) {
    auto space =  boost::hana::integral_constant< char, ' ' >();
    return trim_string_left_c(space, xs);
};


auto trim_string_right_space = [](auto xs) {
    auto space =  boost::hana::integral_constant< char, ' ' >();
    return trim_string_right_c(space, xs);
};


auto trim_string_space = [](auto xs) {
    auto space =  boost::hana::integral_constant< char, ' ' >();
    return trim_string_c(space, xs);
};


}
