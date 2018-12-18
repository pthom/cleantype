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
#include <type_name/details/fp_polyfill/fp_polyfill.hpp>

#include <type_name/details/hana_ext/experimental/detail/type_name_pretty_function.hpp>
#include <type_name/details/type_name_format_whitespace.hpp>
#include <type_name/details/type_name_tree.hpp>

namespace type_name_s
{
    using stringliteral = boost::hana::experimental::type_name_details::stringliteral;

    namespace internal
    {
        // Trick in order to skip having to deal the tedious syntax of parameter packs
        // (Step 1)
        template<typename... T> struct TupleTypeHolder{
        };

        template<typename... T>
        constexpr stringliteral _impl_typeid_hana()
        {
            // Trick in order to skip having to deal the tedious syntax of parameter packs
            // (Step 2)
            return boost::hana::experimental::type_name_details::type_name_impl_stringliteral< TupleTypeHolder<T...> >();
        }


        template<typename... T>
        stringliteral impl_typeid()
        {
            return _impl_typeid_hana<T...>();
        }

        template <typename... T> std::string impl_full()
        {
            stringliteral type_sl_in_tupletypeholder  = impl_typeid<T...>();
            std::string type_in_tupletypeholder =
                boost::hana::experimental::type_name_details::stringliteral_to_string(type_sl_in_tupletypeholder);

            // Trick in order to skip having to deal the tedious syntax of parameter packs
            // (Step 3)
            std::string type_definition;
            {
                auto code_pair_tree_ = type_name::internal::parse_template_tree(type_in_tupletypeholder);
                assert(code_pair_tree_.children_.size() > 0);
                type_definition = type_name::internal::type_children_to_string(code_pair_tree_);
            }

            std::string formatted = type_name::format_whitespace(type_definition);
            return formatted;
        }
    } // namespace internal


    template <typename... T>
    std::string full () {
        return type_name_s::internal::impl_full<T...>();
    }


    template <typename... T> std::string full(T... v) {
        return internal::impl_full<T...>();
    }


    template <typename T> std::string show_details_full(T && v) {
        return std::string("[") + full<T>() + "]" + " = " + fp::show(v);
    }


     template<typename T>
     void ERROR_full() {
       // will make the compiler fail (no call operator), but you can read
       // the name in the output if you squint your eyes
       //constexpr auto t = boost::hana::experimental::type_name<T>()();
       //static_assert(internal::impl_full<T...>() , "truc");

         // TODO !!!!
         //static_assert(boost::hana::experimental::type_name<T>(), "truc");
     }

} // namespace type_name


#define TNS_type_name_full(var) type_name_s::full<decltype(var)>()

#define TNS_show_details_full(var) \
        std::string("[") + TNS_type_name_full(var) + "] " + #var \
        + " = " \
        + fp::show(var)

#define TNS_show_details_full_cont(var) \
        std::string("[") + TN_type_name_full(var) + "] " + #var \
        + " = " \
        + fp::show_cont(var)
