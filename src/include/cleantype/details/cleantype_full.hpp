// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once

#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#include <cxxabi.h>
#endif
#include <cleantype/details/cleantype_fp/fp_base.hpp>
#include <cleantype/details/cleantype_fp/fp_show.hpp>
#include <cleantype/cleantype_configuration.hpp>
#include <cleantype/details/cleantype_eastconst.hpp>
#include <cleantype/details/cleantype_format.hpp>
#include <cleantype/details/cleantype_holder.hpp>
#include <cleantype/details/hana_type_name/type_name_pretty_function.hpp>
#include <cleantype/details/stringutils.hpp>
#include <cstdlib>
#include <memory>
#include <string>

namespace cleantype
{
    using stringliteral = boost::hana::experimental::type_name_details::stringliteral;
    namespace internal
    {
        template <typename T>
        constexpr stringliteral _impl_typeid_hana_literal()
        {
            return boost::hana::experimental::type_name_details::type_name_impl_stringliteral<T>();
        }

        template <typename T>
        std::string _impl_typeid_hana()
        {
            return boost::hana::experimental::type_name_details::stringliteral_to_string(
                _impl_typeid_hana_literal<T>());
        }

        template <typename... T>
        constexpr stringliteral _impl_typeid_hana_inside_holder()
        {
            return boost::hana::experimental::type_name_details::type_name_impl_stringliteral<
                TupleTypeHolder<T...>>();
        }

        template <typename... T>
        stringliteral impl_typeid()
        {
            return _impl_typeid_hana_inside_holder<T...>();
        }

        template <typename... T>
        std::string impl_full()
        {
            stringliteral type_sl_in_tupletypeholder = impl_typeid<T...>();
            std::string type_in_tupletypeholder =
                boost::hana::experimental::type_name_details::stringliteral_to_string(
                    type_sl_in_tupletypeholder);

            std::string type_definition = remove_type_holder_str(type_in_tupletypeholder);

            std::string formatted = cleantype::internal::format_whitespace(type_definition);

            if (CleanConfiguration::GlobalConfig().force_east_const_)
                formatted = cleantype::apply_east_const_typelist(formatted);

            return formatted;
        }
    }  // namespace internal

    template <typename... T>
    std::string full()
    {
        return internal::impl_full<T...>();
    }

    template <typename... T>
    std::string full(T &&... v)
    {
        return full<T...>();
    }

    template <typename T>
    std::string show_details_full(T && v)
    {
        return std::string("[") + full<T>() + "]" + " = " + cleantype_fp::show(v);
    }

}  // namespace cleantype

#define CT_cleantype_full(var) cleantype::full<decltype(var)>()

#define CT_show_details_full(var) \
    std::string("[") + CT_cleantype_full(var) + "] " + #var + " = " + cleantype_fp::show(var)

#define CT_show_details_full_cont(var) \
    std::string("[") + CT_cleantype_full(var) + "] " + #var + " = " + cleantype_fp::show_cont(var)

#define CT_compiler_log_var_type(var) \
    {                                 \
        var.IntentionalError = 42;    \
    }
#define CT_compiler_log_type(T)   \
    {                             \
        T::IntentionalError = 42; \
    }
