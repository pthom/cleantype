// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once
#include <cleantype/cleantype_configuration.hpp>
#include <cleantype/details/cleantype_fp/fp_base.hpp>
#include <cleantype/details/cleantype_fp/fp_tree.hpp>
#include <cleantype/details/cleantype_full.hpp>
#include <cleantype/details/cleantype_holder.hpp>
#include <cleantype/details/cleantype_parse.hpp>
#include <cleantype/details/stringutils.hpp>
#include <deque>
#include <string>

namespace cleantype
{
    namespace internal
    {
        inline code_pair_tree filter_undesirable_type_nodes(code_pair_tree const & xs)
        {
            std::function<bool(const code_pair &)> is_node_desirable =
                [](const code_pair & code_pair) {
                    const std::vector<std::string> undesirable_nodes =
                        cleantype::CleanConfiguration::GlobalConfig().undesirable_type_nodes_;
                    bool found = std::find(undesirable_nodes.begin(),
                                           undesirable_nodes.end(),
                                           cleantype_fp::trim(' ', code_pair.lhs)) !=
                                 undesirable_nodes.end();
                    return !found;
                };

            auto xss = cleantype_fp_tree::tree_keep_if(is_node_desirable, xs);
            return xss;
        }

        inline std::string perform_suppressions(std::string const & typ_name,
                                                std::vector<std::string> const & suppressions)
        {
            std::string result = typ_name;
            for (const auto & v : suppressions)
                result = cleantype_fp::replace_tokens(v, std::string(""), result);
            return result;
        }

        inline std::string perform_replacements(
            std::string const & typ_name, std::map<std::string, std::string> const & replacements)
        {
            std::string result = typ_name;
            for (const auto & kv : replacements)
                result = cleantype_fp::replace_tokens(kv.first, kv.second, result);
            return result;
        }

        inline std::string remove_extra_namespaces(std::string const & typ_name)
        {
            return perform_suppressions(
                typ_name, cleantype::CleanConfiguration::GlobalConfig().suppress_extra_namespaces_);
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
                typ_name, cleantype::CleanConfiguration::GlobalConfig().suppress_custom_);
        }

        inline std::string perform_std_replacements(std::string const & typ_name)
        {
            return perform_replacements(typ_name,
                                        cleantype::CleanConfiguration::GlobalConfig()
                                            .replacements_after_undesirable_node_extractions);
        }

        inline void trim_spaces_inplace(code_pair & xs_io)
        {
            xs_io.lhs = cleantype_fp::trim(' ', xs_io.lhs);
            xs_io.rhs = cleantype_fp::trim(' ', xs_io.rhs);
        }

        inline std::string impl_clean_one_type(std::string const & typ_name)
        {
            std::string typ_namecleaned = remove_struct_class(remove_extra_namespaces(typ_name));
            typ_namecleaned = remove_custom(typ_namecleaned);

            code_pair_tree template_tree = parse_template_tree(typ_namecleaned);
            cleantype_fp_tree::tree_transform_leafs_depth_first_inplace(trim_spaces_inplace,
                                                                        template_tree);
            code_pair_tree template_tree_filtered = filter_undesirable_type_nodes(template_tree);
            std::string template_tree_filtered_str =
                code_pair_tree_to_string(template_tree_filtered);

            std::string final_type = perform_std_replacements(template_tree_filtered_str);
            final_type = format_whitespace(final_type);
            return final_type;
        }

        inline std::string impl_indent_type_tree(const std::string & type_names)
        {
            std::string types_with_holder = add_type_holder_str(type_names);
            code_pair_tree template_tree = parse_template_tree(types_with_holder);
            std::string types_with_holder_indented =
                cleantype_fp_tree::show_tree_lhs_rhs(template_tree,
                                                     make_template_tree_separators(),
                                                     make_template_show_tree_options_with_indent());

            // now, types_with_holder_indented is like this:
            //
            // cleantype::internal::TupleTypeHolder<
            //     Foo<
            //         int,
            //         char
            //     >
            // >
            //
            // --> we remove the lines [0, last]
            //     then we remove the first indentation level
            auto remove_indented_tuple_holder = [](const std::string & type_str) {
                std::vector<std::string> lines = stringutils::split_string(type_str, '\n');
                assert(lines.size() >= 3);
                std::vector<std::string> filtered_lines(lines.begin() + 1, lines.end() - 1);
                std::vector<std::string> unindented_lines = cleantype_fp::transform(
                    [](const std::string & s) { return stringutils::remove_start(s, "    "); },
                    filtered_lines);
                std::string joined_lines = cleantype_fp::join(std::string("\n"), unindented_lines);
                return joined_lines;
            };
            return remove_indented_tuple_holder(types_with_holder_indented);
        }

        inline std::string impl_clean_several_types(std::string const & type_names)
        {
            std::string types_with_holder = add_type_holder_str(type_names);
            std::string types_clean_with_holder = impl_clean_one_type(types_with_holder);
            return remove_type_holder_str(types_clean_with_holder);
        }

        inline std::string impl_indent_if_neeeded(std::string const & type_names)
        {
            std::size_t indent_depth_limit =
                cleantype::CleanConfiguration::GlobalConfig().indent_depth_limit;
            if (indent_depth_limit == 0)
                return type_names;
            std::string types_with_holder = add_type_holder_str(type_names);
            code_pair_tree template_tree = parse_template_tree(types_with_holder);
            std::size_t depth = cleantype_fp_tree::tree_depth(template_tree);
            if (depth > indent_depth_limit + 1)
                return impl_indent_type_tree(type_names);
            else
                return type_names;
        }

        inline std::string impl_clean(std::string const & type_names)
        {
            std::string cleaned = impl_clean_several_types(type_names);
            std::string indented = impl_indent_if_neeeded(cleaned);
            return indented;
        }

    }  // namespace internal

}  // namespace cleantype
