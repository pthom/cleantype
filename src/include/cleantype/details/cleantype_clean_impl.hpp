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
        std::string impl_clean_one_type(std::string const & typ_name, bool remove_type_tuple_holder);

        inline std::vector<std::string> tokenize_params_around_comma(std::string const & params, bool clean_params)
        {
            auto clean_param_if_needed = [&clean_params](std::string const & param) {
                return clean_params ? impl_clean_one_type(param, false) : fp::trim(' ', param);
            };

            std::vector<std::string> result;
            // counts < and > occurrences
            int count = 0;
            std::string current;
            for (const auto c : params)
            {
                if (c == '<')
                    ++count;
                if (c == '>')
                    --count;
                if ( (c == ',') && (count == 0))
                {
                    result.push_back(clean_param_if_needed(current));
                    current = "";
                }
                else
                {
                    current += c;
                }
            }
            result.push_back(clean_param_if_needed(current));
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


        inline std::string code_pair_children_to_string(code_pair_tree const & xs)
        {
            return fp::fp_add::show_tree_children(
                    xs.children_,
                    make_template_tree_separators(),
                    make_template_show_tree_options_no_indent());
        }


        inline std::string code_pair_tree_to_string(code_pair_tree const & xs)
        {
            return fp::fp_add::show_tree_lhs_rhs(
                    xs,
                    make_template_tree_separators(),
                    make_template_show_tree_options_no_indent());
        }


        inline std::string impl_clean_one_type(std::string const & typ_name, bool remove_type_tuple_holder)
        {
            std::string typ_name_trimmed = fp::trim(' ', typ_name);

            std::string typ_namecleaned = remove_struct_class(
                remove_extra_namespaces(typ_name_trimmed));
            typ_namecleaned = remove_custom(typ_namecleaned);

            code_pair_tree template_tree = parse_template_tree(typ_namecleaned);
            fp::fp_add::tree_transform_leafs_depth_first_inplace(trim_spaces_inplace, template_tree);
            code_pair_tree template_tree_filtered = filter_undesirable_template_leafs(template_tree);

            std::string template_tree_filtered_str =
                remove_type_tuple_holder ?
                        code_pair_children_to_string(template_tree_filtered)
                    :   code_pair_tree_to_string(template_tree_filtered);

            std::string final_type = perform_std_replacements(template_tree_filtered_str);
            final_type = add_space_before_ref(final_type);
            return final_type;
        }


        inline std::string impl_clean_several_types_no_indent(std::string const & type_names)
        {
            std::string types_with_holder = "type_tuple_holder< " + type_names + " >";
            bool remove_type_tuple_holder = true;
            std::string types_clean_with_holder = impl_clean_one_type(types_with_holder, remove_type_tuple_holder);
            return types_clean_with_holder;
        }

        inline std::string impl_clean_several_types(std::string const & type_names)
        {
            return impl_clean_several_types_no_indent(type_names);
            //if fp::tree_depth > 3 ...
        }


        inline std::string apply_east_const_impl(std::string const & type_name)
        {
            // Note : this implementation is by no means neither complete nor foolproof
            // It expects types that were preprocessed as inputs (spaces before * and &, etc.)
            // For a more complete implementation, a BNF grammar parse would probably be required
            //
            // By default, cleantype this transformation is not applied (only for the unit tests)
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

    inline std::string apply_east_const(std::string const & type_name)
    {
        std::vector<std::string> types = internal::tokenize_params_around_comma(type_name, false);
        types = fp::transform(internal::apply_east_const_impl, types);
        std::string r = fp::join(", ", types);
        return r;
    }



} // namespace cleantype
