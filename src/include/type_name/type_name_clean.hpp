#pragma once
#include <string>

#include "type_name/type_name_full.hpp"
#include "type_name/details/fp_additions.hpp"

namespace type_name
{


inline fp::fp_add::tree_separators make_template_tree_separators()
{
    fp::fp_add::tree_separators sep;
    sep.open_child = '<';
    sep.close_child = '>';
    sep.separate_siblings = ',';
    return sep;
}

inline fp::fp_add::string_tree parse_template_tree(const std::string &s)
{
    return parse_string_tree(s, make_template_tree_separators(), false, false);
}

inline fp::fp_add::string_tree filter_undesirable_template_noise(const fp::fp_add::string_tree &xs)
{
    std::function<bool(const std::string &)> is_node_desirable =
        [](const std::string &nodename) {
            std::vector<std::string> undesirable_nodes = {
                  "std::char_traits"
                , "struct std::char_traits"
                , "std::allocator"
                , "class std::allocator"
                , "std::less"
                };
            bool found =
                std::find(undesirable_nodes.begin(), undesirable_nodes.end(), fp::trim(' ', nodename)) != undesirable_nodes.end();
            return !found;
        };

    auto xss = fp::fp_add::tree_keep_if(is_node_desirable, xs);
    return xss;
}

struct demangle_typename_params
{
    bool remove_std = false;
};

inline std::string demangle_typename(const std::string & type_name_)
{
    std::string type_name = fp::trim(' ', type_name_);

    std::string type_name_cleaned;
    {
        std::vector<std::pair<std::string, std::string>> replacements = {
              {"::__1", ""}
            , {"::__cxx11", ""}
            , { "struct ", "" }
            , { "class ", "" }
            };
        type_name_cleaned = type_name;
        for (const auto &r : replacements)
            type_name_cleaned = fp::replace_tokens(r.first, r.second, type_name_cleaned);
    }

    fp::fp_add::string_tree template_tree = parse_template_tree(type_name_cleaned);
    fp::fp_add::string_tree template_tree_filtered = filter_undesirable_template_noise(template_tree);

    auto clean1 = fp::fp_add::show_tree(template_tree_filtered, make_template_tree_separators(), false);

    std::string clean2;
    {
        std::vector<std::pair<std::string, std::string>> replacements = {
                  {"const&", "& const"}  // the output is west const
                , {"std::basic_string<char>", "std::string"}
                , {"std::basic_string <char>", "std::string"}
                , {"std::basic_string&<char>", "std::string&"}
                , {"std::basic_string & const<char>", "std::string& const"}
                ,{ "std::basic_string const &<char>", "std::string& const" }          
                , {"class std::basic_string<char>", "std::string"}
                , { ", ", "," }
                ,{ "> >", ">>" }
        };
        clean2 = clean1;
        for (const auto &r : replacements)
            clean2 = fp::replace_tokens(r.first, r.second, clean2);
    }

    return clean2;

    // if (params.remove_std)
    //   demangle_replacements.push_back({"std::", ""});
    //return name;
}


#define var_type_name_clean(var) type_name::demangle_typename( var_type_name_full(var) )

// Also displays the variable name
#define var_name_type_name_clean(var) std::string("type_clean(") + #var + ") = " + var_type_name_clean(var)
#define log_var_name_type_name_clean(var) std::cout << var_name_type_name_clean(var) << std::endl;

#define log_var_str(var) \
        std::string("[") + var_type_name_clean(var) + "] " + #var \
        + " = " \
        + fp::show(var)


#define log_var_str_cont(var) \
        std::string("[") + var_type_name_clean(var) + "] " + #var \
        + " = " \
        + fp::show_cont(var)


#define log_var(var) std::cout << log_var_str(var) << std::endl;
#define log_var_cont(var) std::cout << log_var_str_cont(var) << std::endl;
} // namespace type_name
