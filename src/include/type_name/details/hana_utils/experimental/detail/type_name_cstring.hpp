/*
@file
Defines `boost::hana::experimental::type_name`.
@copyright Louis Dionne 2013-2017
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXPERIMENTAL_DETAIL_TYPE_NAME_CSTRING_HPP
#define BOOST_HANA_EXPERIMENTAL_DETAIL_TYPE_NAME_CSTRING_HPP

#include <type_name/details/hana_utils/experimental/detail/type_name_compiler_capabilities.hpp>
#include <cstddef>
#include <string>

namespace boost {
namespace hana {
namespace experimental {
namespace cstring_utils {

    struct cstring {
        char const* ptr;
        std::size_t length;
    };


#define _HANA_TN_MAKE_CSTRING(string_literal) cstring { string_literal, _HANA_SIZEOF_OR_STRLEN(string_literal);


    constexpr std::size_t constexpr_strlen(char const * s) {
        std::size_t r = 0;
        while(*s++ != '\0')
            r++;
        return r;
    }

constexpr bool cstring_equal(cstring const & cs1, cstring const & cs2) {
    if (cs1.length != cs2.length)
        return false;

    std::size_t idx = 0;
    do {
        char c1 = cs1.ptr[idx];
        char c2 = cs2.ptr[idx];
        if (c1 != c2)
            return false;
        idx++;
    } while (idx < cs1.length);
    return true;
}

    // constexpr bool cstring_equal(cstring const & cs1, cstring const & cs2) {
    //     if (cs1.length != cs2.length)
    //         return false;

    //     std::size_t idx = 0;
    //     do {
    //         char c1 = cs1.ptr[idx];
    //         char c2 = cs2.ptr[idx];
    //         if (c1 != c2)
    //             return false;
    //         idx++;
    //     } while (idx < cs1.length);
    //     return true;
    // }

    constexpr bool cstring_equal_literal(cstring const & cs1, char const * literal) {
        cstring cs2 { literal, constexpr_strlen(literal) };
        return cstring_equal(cs1, cs2);
    }

    inline std::string cstring_to_string(cstring const & cs) {
        char * sz = static_cast<char *>(malloc(cs.length + 1));
#ifdef _MSC_VER
        strncpy_s(sz, cs.length + 1, cs.ptr, cs.length + 1);
#else
        strncpy(sz, cs.ptr, cs.length + 1);
#endif
        sz[cs.length] = '\0';
        std::string r(sz);
        free(sz);
        return r;
    }

} // namespace cstring_utils
} // namespace experimental
} // namespace hana
} // namespace boost

#endif // !BOOST_HANA_EXPERIMENTAL_DETAIL_TYPE_NAME_CSTRING_HPP
