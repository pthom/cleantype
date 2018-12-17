/*
@file
Defines `boost::hana::experimental::type_name`.
@copyright Louis Dionne 2013-2017
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXPERIMENTAL_DETAIL_TYPE_NAME_HPP
#define BOOST_HANA_EXPERIMENTAL_DETAIL_TYPE_NAME_HPP

#include <cstddef>

#ifdef _MSC_VER
#define _HANA_TN__PRETTY_FUNCTION__ __FUNCSIG__
#else
#define _HANA_TN__PRETTY_FUNCTION__ __PRETTY_FUNCTION__
#endif


#if defined(__clang__)
    #define _HANA_TN_PRETTY_FUNCTION_TYPE_PREFIX "boost::hana::hana_type_ext::detail::cstring boost::hana::hana_type_ext::detail::type_name_impl2() [T = "
    #define _HANA_TN_PRETTY_FUNCTION_TYPE_SUFFIX "]"
#elif defined(_MSC_VER)
    #define _HANA_TN_PRETTY_FUNCTION_TYPE_PREFIX  "struct boost::hana::hana_type_ext::detail::cstring __cdecl boost::hana::hana_type_ext::detail::type_name_impl2<"
#define _HANA_TN_PRETTY_FUNCTION_TYPE_SUFFIX ">(void)"
#elif defined(__GNUC__) || defined(__GNUG__)
    #define _HANA_TN_PRETTY_FUNCTION_TYPE_PREFIX "constexpr boost::hana::hana_type_ext::detail::cstring boost::hana::hana_type_ext::detail::type_name_impl2() [with T = "
    #define _HANA_TN_PRETTY_FUNCTION_TYPE_SUFFIX "]"
#else
    #error "No support for this compiler."
#endif


// only clang and MSVC support constexpr __PRETTY_FUNCTION__, gcc does not
#if defined(__clang__) || defined(_MSC_VER)
    #define _HANA_TN_CAN_CONSTEXPR
#endif

// in constexpr mode, strlen is equivalent to sizeof() - 1
#ifdef _HANA_TN_CAN_CONSTEXPR
    #define _HANA_TN_CONSTEXPR_IF_POSSIBLE constexpr
    #define _HANA_SIZEOF_OR_STRLEN(var) sizeof(var) - 1
#else
    #include <cstring> // this include is not needed in constexpr mode, save compilation time
    #define _HANA_TN_CONSTEXPR_IF_POSSIBLE
    #define _HANA_SIZEOF_OR_STRLEN(var) strlen(var)
#endif


namespace boost {
namespace hana {
namespace experimental2 {

    namespace detail {
        struct cstring {
        char const* ptr;
        std::size_t length;
        };

        template <typename T>
        constexpr cstring type_name_impl_cstring() {
            _HANA_TN_CONSTEXPR_IF_POSSIBLE char const* pretty_function = _HANA_TN__PRETTY_FUNCTION__;
            _HANA_TN_CONSTEXPR_IF_POSSIBLE std::size_t total_size = _HANA_SIZEOF_OR_STRLEN(_HANA_TN__PRETTY_FUNCTION__);
            _HANA_TN_CONSTEXPR_IF_POSSIBLE std::size_t prefix_size = _HANA_SIZEOF_OR_STRLEN(_HANA_TN_PRETTY_FUNCTION_TYPE_PREFIX);
            _HANA_TN_CONSTEXPR_IF_POSSIBLE std::size_t suffix_size = _HANA_SIZEOF_OR_STRLEN(_HANA_TN_PRETTY_FUNCTION_TYPE_SUFFIX);
            return {pretty_function + prefix_size, total_size - prefix_size - suffix_size};
        }
    } // end namespace detail

} // namespace experimental2
} // namespace hana
} // namespace boost

#endif // !BOOST_HANA_EXPERIMENTAL_DETAIL_TYPE_NAME_HPP
