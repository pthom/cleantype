#include "doctest.h"
#include <string>
#include <iostream>
#include <boost/any.hpp>
#include <boost/hana/fwd/for_each.hpp>
#include "type_name/details/hana_utils/hana_string_utils.hpp"
#include "type_name/details/type_name_full_s.hpp"

#include <csignal>

#ifndef _NDEBUG
    #ifdef _MSC_VER
        #define TNS_DEBUG_BREAK __debugbreak();
    #else
        #define TNS_DEBUG_BREAK std::raise(SIGINT);
    #endif
#else
    #define TNS_DEBUG_BREAK
#endif

#define REQUIRE_EQ_HANA_STRING(a, b) \
    {                                \
    std::string a_s(a.c_str());      \
    std::string b_s(b);              \
    if (a_s != b_s)                  \
        TNS_DEBUG_BREAK;             \
    REQUIRE_EQ(a_s, b_s);            \
    }


#define LOG_VAR(var) std::cout << "LOG_VAR " << #var << "==>" << var << "<==" << std::endl;


TEST_CASE("reverse")
{
    auto s = BOOST_HANA_STRING("abcdef");
    auto s2 = hana_tn_utils::reverse_boost_hana_string(s);
    std::cout << "Reversed ==>" <<  s2.c_str() << "<==\n";
}


TEST_CASE("trim")
{
    {
        auto s = BOOST_HANA_STRING("  Hello   ");
        auto space =  boost::hana::integral_constant< char, ' ' >();

        auto s1 = hana_tn_utils::trim_string_left_c(space, s);
        REQUIRE_EQ_HANA_STRING(s1, "Hello   ");

        auto s2 = hana_tn_utils::trim_string_right_c(space, s);
        REQUIRE_EQ_HANA_STRING(s2, "  Hello");

        auto s3 = hana_tn_utils::trim_string_c(space, s);
        REQUIRE_EQ_HANA_STRING(s3, "Hello");

        auto s4 = hana_tn_utils::trim_string_space(s);
        REQUIRE_EQ_HANA_STRING(s3, "Hello");
    }
}


TEST_CASE("string_to_char_tuple")
{
    {
      auto s = BOOST_HANA_STRING("a,b");
      auto s_t  = hana_tn_utils::string_to_char_tuple(s);
      auto s_t_s = hana_tn_utils::char_tuple_to_string(s_t);
      static_assert(s == s_t_s, "");
  }
}

TEST_CASE("insert_spaces_after_before")
{
  // {
  //     //auto s = BOOST_HANA_STRING(",abc,def, ghi,");
  //     auto s = BOOST_HANA_STRING("a,b");
  //     auto after_what__comma =  boost::hana::integral_constant< char, ',' >();
  //     auto insert_what__space =  boost::hana::integral_constant< char, ',' >();
  //     auto s1 = hana_tn_utils::insert_char_after(after_what__comma, insert_what__space, s);
  //     //LOG_VAR(s1.c_str());
  //     type_name_s::ERROR_full<decltype(s1)>;
  // }

    {
        auto s = BOOST_HANA_STRING("a,b");
        //type_name_s::ERROR_full<decltype(s)>(); // boost::hana::string<'a', ',', 'b'>
        auto s_t = hana_tn_utils::string_to_char_tuple(s);
        //type_name_s::ERROR_full<decltype(s_t)>(); // boost::hana::tuple<boost::hana::integral_constant<char, 'a'>, boost::hana::integral_constant<char, ','>, boost::hana::integral_constant<char, 'b'> >

        auto should_replace = [](auto xs) {
            return true;
        };
        //type_name_s::ERROR_full<decltype(s_t)>()
        auto replacement = boost::hana::integral_constant<char, '0'>(); // boost::hana::integral_constant<char, 'a'>
        //type_name_s::ERROR_full<decltype(replacement)>();

        auto r_tuple = boost::hana::replace_if(s_t, should_replace, replacement );
        //type_name_s::ERROR_full<decltype(r_tuple)>(); // boost::hana::tuple<int, int, int> !!!! POURQUOI ??????
    }
    {
        auto s = boost::hana::make_tuple('a', 'b', 'c');
        //type_name_s::ERROR_full<decltype(s)>(); // boost::hana::tuple<char, char, char>
        auto should_replace = [](auto xs) {
            return true;
        };

        auto replacement = boost::hana::integral_constant<char, '0'>(); // boost::hana::integral_constant<char, 'a'>
        auto r = boost::hana::replace_if(s, should_replace, replacement );
        //type_name_s::ERROR_full<decltype(r)>(); // boost::hana::tuple<char, char, char> ==> NON
    }


    {
        // BONNE PISTE AVEC TRANSFORM ? NON
        //auto s = boost::hana::make_tuple('a', 'b', 'c');
        auto s = BOOST_HANA_STRING("a,b");
        auto s_t = hana_tn_utils::string_to_char_tuple(s);
        //type_name_s::ERROR_full<decltype(s_t)>(); // boost::hana::tuple<boost::hana::integral_constant<char, 'a'>, boost::hana::integral_constant<char, ','>, boost::hana::integral_constant<char, 'b'> >

       auto after_what__comma =  boost::hana::integral_constant< char, ',' >();

        auto my_transform = [&](auto xs){
            if (xs == after_what__comma)
                return boost::any(boost::hana::integral_constant<char, '0'>());
            else
                return boost::any(xs);
            //auto replacement = boost::hana::integral_constant<char, '0'>(); // boost::hana::integral_constant<char, 'a'>
            //return replacement;
        };

        auto r = boost::hana::transform(s_t, my_transform );
        //type_name_s::ERROR_full<decltype(r)>(); // boost::hana::tuple<boost::hana::integral_constant<char, '0'>, boost::hana::integral_constant<char, '0'>, boost::hana::integral_constant<char, '0'> >
    }

    {
        auto s = BOOST_HANA_STRING("a,b");
        auto s_t = hana_tn_utils::string_to_char_tuple(s);

        std::vector<boost::any> rs;
        rs.push_back(BOOST_HANA_STRING("")); // boost::hana::string<>
        //r + boost::hana::string<'a'>();
        boost::hana::for_each(s, [&](auto xs) {
            // XS : boost::hana::integral_constant<char, 'a'>
            auto xs_s = boost::hana::string<xs>(); // boost::hana::string<'a'>
            //type_name_s::ERROR_full<decltype(r)>();
            // rs.back() + xs_s;
            //auto r2 = r + xs_s; // pas possible, ca fait changer le type de r
        });
    }
}
