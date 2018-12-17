#pragma once

#include <boost/hana.hpp>
#include <boost/hana/reverse.hpp>
#include <boost/hana/fwd/drop_while.hpp>
#include <boost/hana/replace_if.hpp>
#include <boost/hana/fwd/flatten.hpp>

#ifdef _MSC_VER
#define TNS_INLINE_VARIABLES inline
#else
#define TNS_INLINE_VARIABLES
#endif

namespace hana_tn_utils {


TNS_INLINE_VARIABLES auto reverse_boost_hana_string = [](auto s) {
    auto add_reverse = [=](auto xs, auto ys) {
        auto ys_s = boost::hana::string<ys>();
        return ys_s + xs;
    };
    auto reversed = boost::hana::fold_left(s, BOOST_HANA_STRING(""), add_reverse);
    return reversed;
};


TNS_INLINE_VARIABLES auto trim_string_left_c = [](auto trim_what, auto xs) {
    auto should_trim = [&trim_what](auto x) {
        return x == trim_what;
    };
    auto ys = boost::hana::drop_while(xs, should_trim);
    return ys;
};


TNS_INLINE_VARIABLES auto trim_string_right_c = [](auto trim_what, auto xs) {
    auto xs_r = reverse_boost_hana_string(xs);
    auto xs_r_trim = trim_string_left_c(trim_what, xs_r);
    auto trimmed = reverse_boost_hana_string(xs_r_trim);
    return trimmed;
};


TNS_INLINE_VARIABLES auto trim_string_c = [](auto trim_what, auto xs) {
    auto x1 = trim_string_left_c(trim_what, xs);
    auto x2 = trim_string_right_c(trim_what, x1);
    return x2;
};


TNS_INLINE_VARIABLES auto trim_string_left_space = [](auto xs) {
    auto space =  boost::hana::integral_constant< char, ' ' >();
    return trim_string_left_c(space, xs);
};


TNS_INLINE_VARIABLES auto trim_string_right_space = [](auto xs) {
    auto space =  boost::hana::integral_constant< char, ' ' >();
    return trim_string_right_c(space, xs);
};


TNS_INLINE_VARIABLES auto trim_string_space = [](auto xs) {
    auto space =  boost::hana::integral_constant< char, ' ' >();
    return trim_string_c(space, xs);
};


TNS_INLINE_VARIABLES auto string_to_char_tuple = [](auto s)
{
    auto my_add = [](auto xs, auto ys) {
        auto ys_t = boost::hana::make_tuple(ys);
        auto r = boost::hana::flatten( boost::hana::make_tuple(xs, ys_t));
        return r;
    };
    auto r = boost::hana::fold_left(s, boost::hana::make_tuple(), my_add);
    return r;
};


TNS_INLINE_VARIABLES auto char_tuple_to_string = [](auto s)
{
    auto my_add = [](auto xs, auto ys) {
        auto ys_s = boost::hana::string<ys>();
        auto r = xs + ys_s;
        return r;
    };
    auto r = boost::hana::fold_left(s, BOOST_HANA_STRING(""), my_add);
    return r;
};


TNS_INLINE_VARIABLES auto insert_char_after = [](auto insert_what, auto after_which_char, auto xs)
{
    //return xs;
    auto xs_tuple = string_to_char_tuple(xs);

    bool last_char_was_token = false;
    auto should_insert = [](auto xs) {
        return true;
        // bool last_char_was_token_old = last_char_was_token;
        // last_char_was_token = (xs == after_which_char);
        // return last_char_was_token_old;
    };
    auto r_tuple = boost::hana::replace_if(xs_tuple, should_insert, insert_what);
    //auto r_string = char_tuple_to_string(r_tuple);
    return r_tuple;

    //////////

    // bool last_char_was_token = false;
    // auto insert_what_s = boost::hana::string<insert_what>();

    // auto add_with_insert = [&](auto xs, auto ys) {
    //     bool last_char_was_token_old = last_char_was_token;
    //     last_char_was_token = (ys == after_which_char);
    //     // auto ys_s = boost::hana::string<ys>();
    //     // if (last_char_was_token_old)
    //     //     return xs + insert_what + ys;
    //     // else
    //     //     return xs + ys;

    //     auto ys_t = boost::hana::make_tuple(ys);
    //     return xs + ys_t;
    // };
    // //auto r = boost::hana::fold_left(xs, BOOST_HANA_STRING(""), add_with_insert);
    // auto r = boost::hana::fold_left(xs, boost::hana::make_tuple(), add_with_insert);
    // return r;
};


}
