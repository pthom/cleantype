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
        template <typename T, std::size_t... i>
        auto type_name_impl(std::index_sequence<i...>)
        {
            constexpr auto name =
                boost::hana::experimental::type_name_details::type_name_impl_stringliteral<T>();
            return boost::hana::string<*(name.ptr + i)...>{};
        }
    }  // namespace compile_time_internal

    template <typename T>
    auto full_compiletime()
    {
        constexpr auto name =
            boost::hana::experimental::type_name_details::type_name_impl_stringliteral<T>();
        return compile_time_internal::type_name_impl<T>(std::make_index_sequence<name.length>{});
    }
}  // namespace cleantype

#endif  // #if defined(_HANA_TN_CAN_CONSTEXPR)
