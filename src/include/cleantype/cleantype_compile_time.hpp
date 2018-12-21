// This file is part of ConsType : Consistent Type names for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once

#include <cleantype/cleantype.hpp>
#include <cleantype/details/hana_type_name/type_name_pretty_function.hpp>

#if defined(CLEANTYPE_COMPILETIME) && defined(_HANA_TN_CAN_CONSTEXPR)

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
    }

    template <typename T>
    auto compile_time_type_name() {
            constexpr auto name = boost::hana::experimental::type_name_details::type_name_impl_stringliteral<T>();
            return compile_time_internal::type_name_impl<T>(std::make_index_sequence<name.length>{});
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

#ifdef _MSC_VER
#define _CLEANTYPE_INTENTIONAL_ERROR(T) string_to_char_tuple(compile_time_type_name<T...>())
#else
#define _CLEANTYPE_INTENTIONAL_ERROR(T) decltype(compile_time_type_name<T...>())
#endif

    template<typename... T>
    void ERROR_full() {
       static_assert( std::is_integral<_CLEANTYPE_INTENTIONAL_ERROR(T) >(), "" ); // your type can be deciphered via : make 2>&1 | cleantype_compiler_parser [-c | --clean]
    }

#if defined(__clang__)
#define _CLEANTYPE_COMPILETIME_MARKER "cleantype/cleantype_compile_time.hpp:38"
#elif defined(_MSC_VER)
#define _CLEANTYPE_COMPILETIME_MARKER "cleantype_compile_time.hpp(42)" // A revoir, car c'est en fait 2 lignes apres "cleantype_compile_time.hpp(42)"
#else
#error("Compiler not supported")
#endif

}

#endif // #if defined(CLEANTYPE_COMPILETIME) && defined(_HANA_TN_CAN_CONSTEXPR)
