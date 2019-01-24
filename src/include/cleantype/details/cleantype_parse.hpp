// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once
#include <cleantype/details/cleantype_fp/fp_base.hpp>
#include <cleantype/details/cleantype_fp/fp_tree.hpp>
#include <string>

namespace cleantype
{
    namespace internal
    {
        // lhs / rhs explanation :
        // for example in "std::vector<int> const &",
        // - lhs = "std::vector"
        // - rhs = "const &"
        // - int is the lhs of a child node
        using code_pair = cleantype_fp_tree::lhs_rhs;
        using code_pair_tree = cleantype_fp_tree::tree<code_pair>;

        inline cleantype_fp_tree::tree_separators make_template_tree_separators()
        {
            cleantype_fp_tree::tree_separators sep;
            sep.open_child = '<';
            sep.close_child = '>';
            sep.siblings_separator = ',';
            return sep;
        }

        inline cleantype_fp_tree::show_tree_lhs_rhs_options make_template_show_tree_options_impl(
            bool indent)
        {
            cleantype_fp_tree::show_tree_lhs_rhs_options result;
            if (indent)
            {
                result.children_indent = cleantype_fp_tree::children_indent_t::newline_before_child;
                result.siblings_spacing = cleantype_fp_tree::siblings_spacing_t::new_line;
                result.indent = "    ";
            }
            else
            {
                result.children_indent = cleantype_fp_tree::children_indent_t::no_indent;
                result.siblings_spacing = cleantype_fp_tree::siblings_spacing_t::space;
                result.indent = "";
            }
            result.add_space_after_lhs = false;
            result.add_space_before_rhs = true;
            return result;
        }

        inline cleantype_fp_tree::show_tree_lhs_rhs_options
        make_template_show_tree_options_no_indent()
        {
            return make_template_show_tree_options_impl(false);
        }

        inline cleantype_fp_tree::show_tree_lhs_rhs_options
        make_template_show_tree_options_with_indent()
        {
            return make_template_show_tree_options_impl(true);
        }

        inline code_pair_tree parse_template_tree(std::string const & s)
        {
            return parse_lhs_rhs_tree(s, make_template_tree_separators(), true);
        }

        inline std::string code_pair_tree_to_string(code_pair_tree const & xs)
        {
            return cleantype_fp_tree::show_tree_lhs_rhs(
                xs, make_template_tree_separators(), make_template_show_tree_options_no_indent());
        }

    }  // namespace internal

}  // namespace cleantype
