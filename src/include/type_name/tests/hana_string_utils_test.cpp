#include "doctest.h"
#include <string>
#include <iostream>
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
