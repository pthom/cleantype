// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#include "doctest.h"
#include <cleantype/cleantype.hpp>
#include <cleantype/cleantype_compiler_typename.hpp>
#include <cleantype/details/debug_utilities.hpp>

#ifdef _HANA_TN_CAN_CONSTEXPR

TEST_CASE("cleantype_compiler_typename_test")
{
    auto int_type = cleantype::full_compiletime<int>();
    REQUIRE_EQ(cleantype::full(int_type), "boost::hana::string<'i', 'n', 't'> &");

    // Note : there are lots of other tests inside
    // src/include/cleantype/details/hana_type_name/tests/type_name_stringliteral_test.cpp
}

#endif // #ifdef _HANA_TN_CAN_CONSTEXPR
