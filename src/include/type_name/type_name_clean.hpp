#pragma once
#include <string>

#include "type_name/type_name_full.hpp"
#include "type_name/details/fp_additions.hpp"

namespace type_name
{


fp::fp_add::tree_separators make_template_tree_separators()
{
    fp::fp_add::tree_separators sep;
    sep.open_child = '<';
    sep.close_child = '>';
    sep.separate_siblings = ',';
    return sep;
}

fp::fp_add::string_tree parse_template_tree(const std::string &s)
{
    return parse_string_tree(s, make_template_tree_separators(), false, false);
}

fp::fp_add::string_tree filter_undesirable_template_noise(const fp::fp_add::string_tree &xs)
{
    std::function<bool(const std::string &)> is_node_desirable =
        [](const std::string &nodename) {
            std::vector<std::string> undesirable_nodes = {
                "std::char_traits",
                "std::allocator"};
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

std::string demangle_typename(const std::string type_name)
{
    std::string type_name_cleaned;
    {
        std::vector<std::pair<std::string, std::string>> replacements = {
            {"::__1", ""}};
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
            {"std::basic_string<char>", "string"}};
        clean2 = clean1;
        for (const auto &r : replacements)
            clean2 = fp::replace_tokens(r.first, r.second, clean2);
    }

    return clean2;

    // if (params.remove_std)
    //   demangle_replacements.push_back({"std::", ""});
    //return name;
}


} // namespace type_name
