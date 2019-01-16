/*
@file
Defines `boost::hana::experimental::type_name`.
@copyright Louis Dionne 2013-2017
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

/*
This folder contains is a proposed extension
to [Boost.Hana](http://boostorg.github.io/hana/)'s[experimental type_name]

The original version was experimental, and limited to Clang.
This version:
* adds complete support for MSVC
* adds partial support for GCC (runtime only)
* offers compiler capabilities detection
* runs out of the box with or without boost

This extension is being studied :
see PR  https://github.com/boostorg/hana/pull/432
which originates from the author of this project (Pascal Thomet).
*/

#ifndef BOOST_HANA_EXPERIMENTAL_DETAIL_TYPE_NAME_PRERRY_FUNCTION_HPP
#define BOOST_HANA_EXPERIMENTAL_DETAIL_TYPE_NAME_PRERRY_FUNCTION_HPP

#include <cleantype/details/hana_type_name/type_name_compiler_capabilities.hpp>
#include <cleantype/details/hana_type_name/type_name_stringliteral.hpp>
#include <cstddef>

#ifdef _MSC_VER
#define _HANA_TN__PRETTY_FUNCTION__ __FUNCSIG__
#else
#define _HANA_TN__PRETTY_FUNCTION__ __PRETTY_FUNCTION__
#endif

#if defined(__clang__)
#define _HANA_TN_PRETTY_FUNCTION_TYPE_PREFIX                       \
    "boost::hana::experimental::type_name_details::stringliteral " \
    "boost::hana::experimental::type_name_details::type_name_impl_stringliteral() [T = "
#define _HANA_TN_PRETTY_FUNCTION_TYPE_SUFFIX "]"
#elif defined(_MSC_VER)
#define _HANA_TN_PRETTY_FUNCTION_TYPE_PREFIX                                      \
    "struct boost::hana::experimental::type_name_details::stringliteral __cdecl " \
    "boost::hana::experimental::type_name_details::type_name_impl_stringliteral<"
#define _HANA_TN_PRETTY_FUNCTION_TYPE_SUFFIX ">(void)"
#elif defined(__GNUC__) || defined(__GNUG__)
#define _HANA_TN_PRETTY_FUNCTION_TYPE_PREFIX                                 \
    "constexpr boost::hana::experimental::type_name_details::stringliteral " \
    "boost::hana::experimental::type_name_details::type_name_impl_stringliteral() [with T = "
#define _HANA_TN_PRETTY_FUNCTION_TYPE_SUFFIX "]"
#else
#error "No support for this compiler."
#endif

namespace boost
{
    namespace hana
    {
        namespace experimental
        {
            namespace type_name_details
            {
                template <typename T>
                constexpr stringliteral type_name_impl_stringliteral()
                {
                    _HANA_TN_CONSTEXPR_IF_POSSIBLE char const * pretty_function =
                        _HANA_TN__PRETTY_FUNCTION__;
                    _HANA_TN_CONSTEXPR_IF_POSSIBLE std::size_t total_size =
                        _HANA_SIZEOF_OR_STRLEN(_HANA_TN__PRETTY_FUNCTION__);
                    _HANA_TN_CONSTEXPR_IF_POSSIBLE std::size_t prefix_size =
                        _HANA_SIZEOF_OR_STRLEN(_HANA_TN_PRETTY_FUNCTION_TYPE_PREFIX);
                    _HANA_TN_CONSTEXPR_IF_POSSIBLE std::size_t suffix_size =
                        _HANA_SIZEOF_OR_STRLEN(_HANA_TN_PRETTY_FUNCTION_TYPE_SUFFIX);
                    return {pretty_function + prefix_size, total_size - prefix_size - suffix_size};
                    // return {pretty_function, total_size};
                }
            }  // end namespace type_name_details

        }  // namespace experimental
    }      // namespace hana
}  // namespace boost

#endif  // !BOOST_HANA_EXPERIMENTAL_DETAIL_TYPE_NAME_PRERRY_FUNCTION_HPP
