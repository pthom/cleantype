// Note : this file is unused in this library. It is here only in order to prepare a PR to Boost.Hana.
/*
@file
Defines `boost::hana::experimental::type_name`.
@copyright Louis Dionne 2013-2017
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXPERIMENTAL_TYPE_NAME_HPP
#define BOOST_HANA_EXPERIMENTAL_TYPE_NAME_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/string.hpp>
#include <type_name/details/hana_ext/experimental/detail/type_name_pretty_function.hpp>

#include <utility>


BOOST_HANA_NAMESPACE_BEGIN  namespace experimental {
    namespace type_name_details {
        template <typename T, std::size_t ...i>
        auto type_name_impl(std::index_sequence<i...>) {
            constexpr auto name = type_name_details::type_name_impl_stringliteral<T>();
            auto r = boost::hana::string<*(name.ptr + i)...>{};
            return r;
        }
    } // end namespace type_name_details

    template <typename T>
    auto type_name() {
        #ifdef _HANA_TN_CAN_CONSTEXPR
            constexpr auto name = type_name_details::type_name_impl_stringliteral<T>();
            return type_name_details::type_name_impl<T>(std::make_index_sequence<name.length>{});
        #else
            auto as_cstring = type_name_details::type_name_impl_stringliteral<T>();
            std::string name_full = std::string(as_cstring.ptr);
            std::string name_cut = name_full.substr(0, as_cstring.length);
            return name_cut;
        #endif
    }
} BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_EXPERIMENTAL_TYPE_NAME_HPP
