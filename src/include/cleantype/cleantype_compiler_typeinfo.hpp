// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once

#include <cleantype/cleantype.hpp>
#include <cleantype/details/hana_type_name/type_name_pretty_function.hpp>

#if defined(_HANA_TN_CAN_CONSTEXPR)

#include <boost/hana.hpp>
#include <boost/hana/string.hpp>

namespace cleantype
{
    namespace compile_time_internal
    {
        template <typename T, std::size_t ...i>
        auto type_name_impl(std::index_sequence<i...>) {
            constexpr auto name =
                boost::hana::experimental::type_name_details::
                    type_name_impl_stringliteral<T>();
            return boost::hana::string<*(name.ptr + i)...>{};
        }

        auto string_to_char_tuple = [](auto s)
        {
            auto my_add = [](auto xs, auto ys) {
                auto ys_t = boost::hana::make_tuple(ys);
                auto r = boost::hana::flatten(boost::hana::make_tuple(xs, ys_t));
                return r;
            };
            auto r = boost::hana::fold_left(s, boost::hana::make_tuple(), my_add); // MSVC MARKER
            return r;
        };

    }

    template <typename T>
    auto compile_time_type_name() {
            constexpr auto name = boost::hana::experimental::type_name_details::type_name_impl_stringliteral<T>();
            return compile_time_internal::type_name_impl<T>(std::make_index_sequence<name.length>{});
        }

    template<typename... T> constexpr bool IntentionalError()
    {
        return false;
    };


#ifdef _MSC_VER
#define _CLEANTYPE_OUTPUT(T) cleantype::compile_time_internal::string_to_char_tuple(cleantype::compile_time_type_name<T...>())
#else
#define _CLEANTYPE_OUTPUT(T) decltype(cleantype::compile_time_type_name<T...>())
#endif

    template<typename... T>
    void ERROR_full() {
       static_assert( IntentionalError<_CLEANTYPE_OUTPUT(T) >(), "" ); // your type can be deciphered via : make 2>&1 | cleantype_compiler_parser [-c | --clean]
    }

#ifdef _MSC_VER
#define TN_ERROR_full(T) static_assert( std::is_integral<_CLEANTYPE_OUTPUT(T) >(), "" );
#define TN_ERROR_full_var(var) static_assert( std::is_integral<_CLEANTYPE_OUTPUT( decltype(var) ) >(), "" );
#else
#define TN_ERROR_full(T) cleantype::ERROR_full<T>();
#define TN_ERROR_full_var(var) cleantype::ERROR_full<decltype(var)>();
#endif

}

#endif // #if defined(_HANA_TN_CAN_CONSTEXPR)
