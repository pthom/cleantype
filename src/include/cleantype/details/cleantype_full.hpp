// This file is part of ConsType : Consistent Type names for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once

#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>
#include <string>
#include <cleantype/details/fp_polyfill/fp_polyfill.hpp>

#include <cleantype/details/hana_ext/experimental/detail/type_name_pretty_function.hpp>
#include <cleantype/details/cleantype_format_whitespace.hpp>
#include <cleantype/details/cleantype_clean_impl.hpp>
#include <cleantype/cleantype_configuration.hpp>

namespace cleantype
{
    using stringliteral = boost::hana::experimental::type_name_details::stringliteral;

    namespace internal
    {
        // Trick in order to skip having to deal the tedious syntax of parameter packs
        template<typename... T> struct TupleTypeHolder{
        };


        template<typename T>
        constexpr stringliteral _impl_typeid_hana_no_holder_literal()
        {
            return boost::hana::experimental::type_name_details::type_name_impl_stringliteral<T>();
        }

        template<typename T>
        std::string _impl_typeid_hana_no_holder()
        {
            return boost::hana::experimental::type_name_details::stringliteral_to_string(
                _impl_typeid_hana_no_holder_literal<T>()
            );
        }


        template<typename... T>
        constexpr stringliteral _impl_typeid_hana()
        {
            return boost::hana::experimental::type_name_details::type_name_impl_stringliteral< TupleTypeHolder<T...> >();
        }


        template<typename... T>
        stringliteral impl_typeid()
        {
            return _impl_typeid_hana<T...>();
        }


        inline std::string extract_type_from_tupletypeholder(std::string const & typ_name)
        {
            auto code_pair_tree_ = cleantype::internal::parse_template_tree(typ_name);
            assert(code_pair_tree_.children_.size() > 0);
            std::string extracted_type = cleantype::internal::type_children_to_string(code_pair_tree_);
            return extracted_type;
        }


        template <typename... T> std::string impl_full()
        {
            stringliteral type_sl_in_tupletypeholder  = impl_typeid<T...>();
            std::string type_in_tupletypeholder =
                boost::hana::experimental::type_name_details::stringliteral_to_string(type_sl_in_tupletypeholder);

            std::string type_definition = extract_type_from_tupletypeholder(type_in_tupletypeholder);

            std::string formatted = cleantype::format_whitespace(type_definition);

            if (CleanConfiguration::GlobalConfig().force_east_const_)
                formatted = cleantype::apply_east_const(formatted);

            return formatted;
        }
    } // namespace internal


    template <typename... T>
    std::string full () {
        return internal::impl_full<T...>();
    }


    template <typename... T> std::string full(T&&... v) {
        return full<T...>();
    }


    template <typename T> std::string show_details_full(T && v) {
        return std::string("[") + full<T>() + "]" + " = " + fp::show(v);
    }


     template<typename... T>
     void ERROR_full() {
       // will make the compiler fail (no call operator), but you can read
       // the name in the output if you squint your eyes
       constexpr auto t = internal::impl_typeid<T...>()();
       static_assert(internal::impl_full<T...>() , "truc");

         // TODO !!!!
         //static_assert(boost::hana::experimental::cleantype<T>(), "truc");
     }

} // namespace cleantype


#define TN_cleantype_full(var) cleantype::full<decltype(var)>()

#define TN_show_details_full(var) \
        std::string("[") + TNS_cleantype_full(var) + "] " + #var \
        + " = " \
        + fp::show(var)

#define TN_show_details_full_cont(var) \
        std::string("[") + TN_cleantype_full(var) + "] " + #var \
        + " = " \
        + fp::show_cont(var)
