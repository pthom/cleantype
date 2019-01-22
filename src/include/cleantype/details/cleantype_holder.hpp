// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once
#include <cleantype/details/cleantype_fp/fp_base.hpp>
#include <cleantype/details/stringutils.hpp>
#include <string>

namespace cleantype
{
    namespace internal
    {
        // Trick in order to avoid having to deal the tedious syntax of parameter packs
        template <typename... T>
        struct TupleTypeHolder
        {
        };

        inline std::string add_type_holder_str(const std::string & type_names)
        {
#ifdef _MSC_VER
            const std::string start = "struct cleantype::internal::TupleTypeHolder<";
#else
            const std::string start = "cleantype::internal::TupleTypeHolder<";
#endif
            const std::string end = ">";
            return start + type_names + end;
        }

        inline std::string remove_type_holder_str(const std::string & types_inside_holder)
        {
            std::string r = types_inside_holder;
            if (stringutils::starts_with(r, "struct cleantype::internal::TupleTypeHolder<"))
                r = stringutils::remove_start(r, "struct cleantype::internal::TupleTypeHolder<");
            if (stringutils::starts_with(r, "cleantype::internal::TupleTypeHolder<"))
                r = stringutils::remove_start(r, "cleantype::internal::TupleTypeHolder<");
            if (stringutils::ends_with(r, ">"))
                r = stringutils::remove_end(r, ">");
            return r;
        }

        // splits a string containings a list of comma separated types into
        // a vector of types
        // example :
        // split_types("int, map<char, int>") => ["int", "map<char, int>"]
        inline std::vector<std::string> split_types(std::string const & type_names)
        {
            std::vector<std::string> result;
            // counts < and > occurrences
            int count = 0;
            std::string current;
            for (const auto c : type_names)
            {
                if (c == '<')
                    ++count;
                if (c == '>')
                    --count;
                if ((c == ',') && (count == 0))
                {
                    result.push_back(cleantype_fp::trim(' ', current));
                    current = "";
                }
                else
                {
                    current += c;
                }
            }
            result.push_back(cleantype_fp::trim(' ', current));
            return result;
        }

    }  // namespace internal
}  // namespace cleantype