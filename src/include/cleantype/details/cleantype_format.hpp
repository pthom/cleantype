// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once
#include <cleantype/details/cleantype_eastconst.hpp>
#include <cleantype/details/cleantype_holder.hpp>
#include <cleantype/details/cleantype_parse.hpp>
#include <cleantype/details/stringutils.hpp>
#include <string>

namespace cleantype
{
    namespace internal
    {
        // r = stringutils::replace_tokens("__cdecl ", "", r);
        // r = stringutils::replace_tokens("struct ", "", r);
        // r = stringutils::replace_tokens("class ", "", r);

        inline std::string format_whitespace(const std::string & str_type)
        {
            std::string r = str_type;
            r = stringutils::replace_tokens("\t", " ", r);
            r = stringutils::replace_tokens("\r\n", " ", r);
            r = stringutils::replace_tokens("\n", " ", r);
            r = stringutils::replace_tokens("  ", " ", r);
            r = stringutils::replace_tokens("  ", " ", r);
            r = stringutils::replace_tokens("  ", " ", r);
            r = stringutils::insert_spaces_after(',', r);
            r = stringutils::insert_spaces_before_after('&', r);
            r = stringutils::insert_spaces_before_after('*', r);
            r = stringutils::remove_spaces_before_after(')', r);
            r = stringutils::remove_spaces_before_after('(', r);
            r = stringutils::remove_spaces_before('>', r);
            r = stringutils::remove_spaces_after('<', r);
            r = stringutils::remove_spaces_after('[', r);
            r = stringutils::remove_spaces_before(']', r);
            r = stringutils::insert_spaces_before('[', r);
            r = stringutils::insert_spaces_after(']', r);
            r = stringutils::replace_tokens("*&", "* &", r);
            r = stringutils::replace_tokens("&*", "& *", r);
            r = stringutils::replace_tokens("& &", "&&", r);
            r = stringutils::replace_tokens("[ ]", "[]", r);
            r = stringutils::replace_tokens(" ,", ",", r);
            r = stringutils::replace_tokens("* *", "**", r);
            r = stringutils::trim(' ', r);
            return r;
        }

        inline void format_whitespace_code_pair_inplace(code_pair & cp)
        {
            cp.lhs = format_whitespace(cp.lhs);
            cp.rhs = format_whitespace(cp.rhs);
        }

        inline void force_east_const_code_pair_inplace(code_pair & cp)
        {
            const std::string separator = " __ChIlDren__ ";
            // Example :
            // Given the type : "const vector<map<string, int>> *&"
            //   Then lhs = "const vector" and rhs = *&
            //    (and children will contain "map<string, int>")

            // in the example : lhs_rhs = "const vector ___ *&"
            std::string lhs_rhs = cp.lhs + separator + cp.rhs;
            // in the example : lhs_rhs_transformed = "vector ___ *& const"
            std::string lhs_rhs_transformed = apply_east_const_one_type(lhs_rhs);

            std::size_t pos = lhs_rhs_transformed.find(separator);
            if (pos != std::string::npos)
            {
                cp.lhs = cleantype::stringutils::trim(' ', lhs_rhs_transformed.substr(0, pos));
                cp.rhs = cleantype::stringutils::trim(
                    ' ', lhs_rhs_transformed.substr(pos + separator.size()));
            }
        }

        template <typename FnPerType>
        inline std::string apply_transform_per_type(FnPerType && fn_per_type,
                                                    const std::string & type_names)
        {
            std::string types_with_holder = add_type_holder_str(type_names);
            code_pair_tree template_tree = parse_template_tree(types_with_holder);
            cleantype_fp_tree::tree_transform_leafs_depth_first_inplace(fn_per_type, template_tree);
            std::string types_with_holder_transformed = code_pair_tree_to_string(template_tree);
            return remove_type_holder_str(types_with_holder_transformed);
        }

        inline std::string format_impl(const std::string & type_names)
        {
            std::string r =
                apply_transform_per_type(format_whitespace_code_pair_inplace, type_names);
            if (force_east_const())
                r = apply_transform_per_type(force_east_const_code_pair_inplace, r);
            return r;
        }

    }  // namespace internal

    inline std::string format(const std::string & type_names)
    {
        return internal::format_impl(type_names);
    }
}  // namespace cleantype
