// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once
#include <string>
#include <deque>
#include <cleantype/details/cleantype_full.hpp>
#include <cleantype/details/fp_polyfill/fp_polyfill.hpp>
#include <cleantype/details/fp_polyfill/fp_additions.hpp>
#include <cleantype/cleantype_configuration.hpp>
#include <cleantype/details/stringutils.hpp>

namespace cleantype
{

    namespace internal
    {
        std::string impl_clean_one_type(std::string const & typ_name);
        std::string add_type_holder_str(std::string const & type_names);
        std::string remove_type_holder_str(std::string const & type_name);

        inline std::vector<std::string> clean_several_types(const std::vector<std::string> &types)
        {
            return fp::transform(impl_clean_one_type, types);
        }


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
                if ( (c == ',') && (count == 0))
                {
                    result.push_back(fp::trim(' ', current));
                    current = "";
                }
                else
                {
                    current += c;
                }
            }
            result.push_back(fp::trim(' ', current));
            return result;
        }


        // lhs / rhs explanation :
        // for example in "std::vector<int> const &",
        // - lhs = "std::vector"
        // - rhs = "const &"
        // - int is the lhs of a child node
        using code_pair = fp::fp_add::lhs_rhs;
        using code_pair_tree = fp::tree<code_pair>;


        inline fp::fp_add::tree_separators make_template_tree_separators()
        {
            fp::fp_add::tree_separators sep;
            sep.open_child = '<';
            sep.close_child = '>';
            sep.siblings_separator = ',';
            return sep;
        }


        inline fp::fp_add::show_tree_lhs_rhs_options make_template_show_tree_options_impl(bool indent)
        {
            fp::fp_add::show_tree_lhs_rhs_options result;
            if (indent)
            {
                result.add_new_lines_before_children = true;
                result.siblings_spacing = fp::fp_add::siblings_spacing_t::new_line;
                result.indent = "    ";
            }
            else
            {
                result.add_new_lines_before_children = false;
                result.siblings_spacing = fp::fp_add::siblings_spacing_t::space;
                result.indent = "";
            }
            result.add_space_after_lhs = false;
            result.add_space_before_rhs = true;
            return result;
        }


        inline fp::fp_add::show_tree_lhs_rhs_options make_template_show_tree_options_no_indent()
        {
            return make_template_show_tree_options_impl(false);
        }

        inline fp::fp_add::show_tree_lhs_rhs_options make_template_show_tree_options_with_indent()
        {
            return make_template_show_tree_options_impl(true);
        }

        inline code_pair_tree parse_template_tree(std::string const &s)
        {
            return parse_lhs_rhs_tree(s, make_template_tree_separators(), true);
        }


        inline code_pair_tree filter_undesirable_template_leafs(code_pair_tree const &xs)
        {
            std::function<bool(const code_pair &)> is_node_desirable =
                [](const code_pair &code_pair) {
                    const std::vector<std::string> undesirable_nodes =
                        cleantype::CleanConfiguration::GlobalConfig().undesirable_type_nodes_;
                    bool found =
                        std::find(undesirable_nodes.begin(), undesirable_nodes.end(), fp::trim(' ', code_pair.lhs)) != undesirable_nodes.end();
                    return !found;
                };

            auto xss = fp::fp_add::tree_keep_if(is_node_desirable, xs);
            return xss;
        }

        inline std::string perform_suppressions(
            std::string const & typ_name,
            std::vector<std::string> const & suppressions)
        {
            std::string result = typ_name;
            for (const auto &v : suppressions)
                result = fp::replace_tokens(v, "", result);
            return result;
        }


        inline std::string perform_replacements(
            std::string const & typ_name,
            std::vector<std::array<std::string, 2>> const & replacements)
        {
            std::string result = typ_name;
            for (const auto &r : replacements)
                result = fp::replace_tokens(r[0], r[1], result);
            return result;
        }


        inline std::string remove_extra_namespaces(std::string const & typ_name)
        {
            return perform_suppressions(
                typ_name,
                cleantype::CleanConfiguration::GlobalConfig().suppress_extra_namespaces_);
        }


        inline std::string remove_struct_class(std::string const & typ_name)
        {
            return perform_suppressions(
                typ_name,
                cleantype::CleanConfiguration::GlobalConfig().suppress_extract_struct_class_);
        }

        inline std::string remove_custom(std::string const & typ_name)
        {
            return perform_suppressions(
                typ_name,
                cleantype::CleanConfiguration::GlobalConfig().suppress_custom_);
        }


        inline std::string perform_std_replacements(std::string const & typ_name)
        {
            return perform_replacements(typ_name,
                cleantype::CleanConfiguration::GlobalConfig().replacements_after_undesirable_node_extractions
            );
        }


        inline void trim_spaces_inplace(code_pair & xs_io)
        {
            xs_io.lhs = fp::trim(' ', xs_io.lhs);
            xs_io.rhs = fp::trim(' ', xs_io.rhs);
        }


        inline std::string add_space_before_ref(std::string const & typ_name)
        {
            std::string result = "";
            bool space_or_ref_before = false;
            for (auto c : typ_name)
            {
                if (c == '&')
                    if (!space_or_ref_before)
                        result = result + " ";
                space_or_ref_before = ( (c == ' ') || (c == '&'));
                result = result + c;
            }
            return result;
        }


        inline std::string code_pair_tree_to_string(code_pair_tree const & xs)
        {
            return fp::fp_add::show_tree_lhs_rhs(
                    xs,
                    make_template_tree_separators(),
                    make_template_show_tree_options_no_indent());
        }


        inline std::string impl_clean_one_type(std::string const & typ_name)
        {
            std::string typ_name_trimmed = fp::trim(' ', typ_name);

            std::string typ_namecleaned = remove_struct_class(
                remove_extra_namespaces(typ_name_trimmed));
            typ_namecleaned = remove_custom(typ_namecleaned);

            code_pair_tree template_tree = parse_template_tree(typ_namecleaned);
            fp::fp_add::tree_transform_leafs_depth_first_inplace(trim_spaces_inplace, template_tree);
            code_pair_tree template_tree_filtered = filter_undesirable_template_leafs(template_tree);
            std::string template_tree_filtered_str = code_pair_tree_to_string(template_tree_filtered);

            std::string final_type = perform_std_replacements(template_tree_filtered_str);
            final_type = add_space_before_ref(final_type);
            return final_type;
        }


        inline std::string impl_clean_several_types_no_indent(std::string const & type_names)
        {
            std::string types_with_holder = add_type_holder_str(type_names);
            std::string types_clean_with_holder = impl_clean_one_type(types_with_holder);
            return remove_type_holder_str(types_clean_with_holder);
        }


        inline std::string impl_clean_several_types(std::string const & type_names)
        {
            return impl_clean_several_types_no_indent(type_names);
            //if fp::tree_depth > 3 ...
        }


        inline std::vector<std::string> _split_string(std::string const & s, char delimiter)
        {
            std::vector<std::string> tokens;
            std::string token;
            std::istringstream tokenStream(s);
            while (std::getline(tokenStream, token, delimiter))
                tokens.push_back(token);
            return tokens;
        }


    } // namespace internal


} // namespace cleantype
