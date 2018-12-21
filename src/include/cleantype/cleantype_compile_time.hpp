// This file is part of ConsType : Consistent Type names for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once

#include <cleantype/cleantype.hpp>
#include <cleantype/details/hana_type_name/type_name_pretty_function.hpp>

#if defined(CLEANTYPE_COMPILETIME) && defined(_HANA_TN_CAN_CONSTEXPR)

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

    template<typename... T>
    void ERROR_full() {
       // will make the compiler fail (no call operator), but you can read
       // the name in the output if you squint your eyes
       // The line number of the instruction below should be noted inside _CLEANTYPE_COMPILETIME_MARKER
       auto intentional_error = compile_time_type_name<T...>();
       intentional_error(); // your type can be deciphered via : make 2>&1 | cleantype_compiler_parser [-c | --clean]
    }

#define _CLEANTYPE_COMPILETIME_MARKER "cleantype/cleantype_compile_time.hpp:38"

}

#endif // #if defined(CLEANTYPE_COMPILETIME) && defined(_HANA_TN_CAN_CONSTEXPR)
