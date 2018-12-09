#pragma once
#include <string>
#include <deque>
#include "type_name/details/type_name_full.hpp"
#include "type_name/details/fp_polyfill/fp_additions.hpp"

namespace type_name
{

    namespace internal
    {
        using code_pair = fp::fp_add::lhs_rhs; // for example in "std::vector<int> const &", then start = "std::vector" and end = "const &"
        using code_pair_tree = fp::tree<code_pair>;


        inline fp::fp_add::tree_separators make_template_tree_separators()
        {
            fp::fp_add::tree_separators sep;
            sep.open_child = '<';
            sep.close_child = '>';
            sep.separate_siblings = ',';
            return sep;
        }


        inline fp::fp_add::show_tree_lhs_rhs_options make_template_show_tree_options()
        {
            fp::fp_add::show_tree_lhs_rhs_options result;
            result.add_new_lines = false;
            result.add_space_between_siblings = true;
            result.indent = "";
            result.add_space_after_lhs = false;
            result.add_space_before_rhs = true;
            return result;
        }


        inline code_pair_tree parse_template_tree(const std::string &s)
        {
            return parse_lhs_rhs_tree(s, make_template_tree_separators(), false, false);
        }


        inline code_pair_tree filter_undesirable_template_leafs(const code_pair_tree &xs)
        {
            std::function<bool(const code_pair &)> is_node_desirable =
                [](const code_pair &code_pair) {
                    std::vector<std::string> undesirable_nodes = {
                        "std::char_traits"
                        , "struct std::char_traits"
                        , "std::allocator"
                        , "class std::allocator"
                        , "std::less"
                        };
                    bool found =
                        std::find(undesirable_nodes.begin(), undesirable_nodes.end(), fp::trim(' ', code_pair.lhs)) != undesirable_nodes.end();
                    return !found;
                };

            auto xss = fp::fp_add::tree_keep_if(is_node_desirable, xs);
            return xss;
        }


        inline std::string perform_replacements(
            const std::string & typ_name,
            const std::vector<std::pair<std::string, std::string>> replacements)
        {
            std::string result = typ_name;
            for (const auto &r : replacements)
                result = fp::replace_tokens(r.first, r.second, result);
            return result;
        }


        inline std::string remove_extra_namespaces(const std::string & typ_name)
        {
            return perform_replacements(typ_name,
            {
                { "::__1", "" }
                , { "::__cxx11", "" }
            });
        }


        inline std::string remove_struct_class(const std::string & typ_name)
        {
            return perform_replacements(typ_name,
            {
                { "struct ", "" }
                ,{ "class ", "" }
            });
        }


        inline std::string perform_std_replacements(const std::string & typ_name)
        {
            return perform_replacements(typ_name,
            {
                {"std::basic_string<char>", "std::string"}
                , {"std::basic_string <char>", "std::string"}
                , {"std::basic_string &<char>", "std::string &"}
                //, {"std::basic_string & const<char>", "std::string& const"}
                , { "std::basic_string const &<char>", "std::string const &" }
                , {"class std::basic_string<char>", "std::string"}
                , {"std::string const & __ptr64", "std::string const &"}
                , { "> >", ">>" }
            });
        }


        inline std::vector<std::string> _split_string(const std::string& s, char delimiter)
        {
            std::vector<std::string> tokens;
            std::string token;
            std::istringstream tokenStream(s);
            while (std::getline(tokenStream, token, delimiter))
                tokens.push_back(token);
            return tokens;
        }


        inline void trim_spaces_inplace(code_pair & xs_io)
        {
            xs_io.lhs = fp::trim(' ', xs_io.lhs);
            xs_io.rhs = fp::trim(' ', xs_io.rhs);
        }


        inline std::string add_space_before_ref(const std::string & typ_name)
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


        inline std::string impl_clean(const std::string & typ_name)
        {
            std::string typ_name_trimmed = fp::trim(' ', typ_name);

            std::string typ_namecleaned = remove_struct_class(
                remove_extra_namespaces(typ_name_trimmed));

            code_pair_tree template_tree = parse_template_tree(typ_namecleaned);
            fp::fp_add::tree_transform_leafs_depth_first_inplace(trim_spaces_inplace, template_tree);
            code_pair_tree template_tree_filtered = filter_undesirable_template_leafs(template_tree);

            auto template_tree_filtered_str = fp::fp_add::show_tree_lhs_rhs(
                template_tree_filtered,
                make_template_tree_separators(),
                make_template_show_tree_options());

            std::string final_type = perform_std_replacements(template_tree_filtered_str);
            final_type = add_space_before_ref(final_type);
            //final_type = DISABLED_apply_east_const(final_type);
            return final_type;
        }


    } // namespace internal

    //////////////////////////////
    // Start of public API
    //////////////////////////////

    inline std::string clean(const std::string & typ_name)
    {
        return internal::impl_clean(typ_name);
    }


} // namespace type_name

#define m_type_name_clean(var) type_name::clean( m_type_name_full(var) )


#define show_details(var) \
        std::string("[") + m_type_name_clean(var) + "] " + #var \
        + " = " \
        + fp::show(var)


#define show_details_cont(var) \
        std::string("[") + m_type_name_clean(var) + "] " + #var \
        + " = " \
        + fp::show_cont(var)
