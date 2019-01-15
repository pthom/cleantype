// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once
#include <string>
#include <deque>
#include <cleantype/details/cleantype_full.hpp>
#include <cleantype/details/cleantype_clean_impl.hpp>
#include <cleantype/details/fp_polyfill/fp_polyfill.hpp>
#include <cleantype/details/stringutils.hpp>

namespace cleantype
{

    namespace internal
    {
        inline std::string apply_east_const_one_type(std::string const & type_name)
        {
            // Note : this implementation is by no means neither complete nor foolproof
            // It expects types that were preprocessed as inputs (spaces before * and &, etc.)
            // For a more complete implementation, maybe BNF grammar parsing would be required
            //
            // By default, this transformation is not applied : it is only for the unit tests
            // consistency accross platforms. I am not an east-const zealot.
            namespace stringutils = cleantype::stringutils;

            if (type_name.empty())
                return "";
            if (stringutils::ends_with(type_name, "const") && (!stringutils::starts_with(type_name, "const ")))
                return type_name;

            // const T * const => T const * const
            if (stringutils::starts_ends_with(type_name, "const ", " * const"))
            {
                auto r = stringutils::remove_start_end(type_name, "const ", " * const");
                r = r + " const * const";
                return r;
            }

            // const T * const & => T const * const &
            if (stringutils::starts_ends_with(type_name, "const ", " * const &"))
            {
                auto r = stringutils::remove_start_end(type_name, "const ", " * const &");
                r = r + " const * const &";
                return r;
            }


            // const T * & => T const * &
            if (stringutils::starts_ends_with(type_name, "const ", " * &"))
            {
                auto r = stringutils::remove_start_end(type_name, "const ", " * &");
                r = r + " const * &";
                return r;
            }

            // const T & => T const &
            if (stringutils::starts_ends_with(type_name, "const ", " &"))
            {
                auto r = stringutils::remove_start_end(type_name, "const ", " &");
                r = r + " const &";
                return r;
            }

            // const T * => T const *
            if (stringutils::starts_ends_with(type_name, "const ", " *"))
            {
                auto r = stringutils::remove_start_end(type_name, "const ", " *");
                r = r + " const *";
                return r;
            }

            // const * T => T const *
            if (stringutils::starts_with(type_name, "const * "))
            {
                auto r = stringutils::remove_start(type_name, "const * ");
                r = r + " const *";
                return r;
            }

            // const T => T const
            if (stringutils::starts_with(type_name, "const "))
            {
                auto r = stringutils::remove_start(type_name, "const ");
                r = r + " const";
                return r;
            }

            return type_name;
        }

    } // namespace internal

    inline std::string apply_east_const_typelist(std::string const & type_names)
    {
        std::vector<std::string> types = internal::split_types(type_names);
        types = fp::transform(internal::apply_east_const_one_type, types);
        std::string r = fp::join(", ", types);
        return r;
    }

} // namespace cleantype
