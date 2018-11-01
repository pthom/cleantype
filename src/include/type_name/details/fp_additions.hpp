#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <deque>
#include <vector>
#include <algorithm>
#include <functional>

#include "type_name/details/fp_polyfill.hpp"
#include "type_name/details/fp_incompat.hpp"


// Additions to fplus
namespace fp
{
namespace fp_add
{
using string_tree = tree<std::string>;
struct tree_separators {
    char open_child = '<';
    char close_child = '>';
    char separate_siblings = ',';
};

string_tree parse_string_tree(
    const std::string &s,
    const tree_separators & separators,
    bool remove_space_after_tok,
    bool remove_space)
{
    std::deque<char> fifo_letters;
    {
        for (const auto & letter: s)
            fifo_letters.push_back(letter);
    }

    string_tree result{"", {}};
    string_tree *current = &result;
    std::vector<string_tree *> parents;

    bool is_first_after_tok = false;
    while (!fifo_letters.empty())
    {
        char c = fifo_letters.front();

        fifo_letters.pop_front();
        if (isspace(c))
        {
            if (remove_space)
                continue;
            if (remove_space_after_tok && is_first_after_tok)
                continue;
        }
        if (c == separators.open_child)
        {
            string_tree child{"", {}};
            current->children_.push_back(child);
            parents.push_back(current);
            current = &(parents.back()->children_.back());
            is_first_after_tok = true;
        }
        else if (c == separators.separate_siblings)
        {
            string_tree brother{"", {}};
            parents.back()->children_.push_back(brother);
            current = &(parents.back()->children_.back());
            is_first_after_tok = true;
        }
        else if (c == separators.close_child)
        {
            current = parents.back();
            parents.pop_back();
            is_first_after_tok = true;
        }
        else
        {
            current->value_ += c;
            is_first_after_tok = false;
        }
    }
    return result;
}

template <typename T>
tree<T> tree_keep_if(std::function<bool(const T &)> f, const tree<T> &xs)
{
    tree<T> result = xs;

    result.children_ = fp::keep_if([f](const tree<T> &xxs) {
        return f(xxs.value_);
    },
    result.children_);

    std::vector<tree<T>> children_filtered_grandchhildren;
    for (auto &child : result.children_)
    {
        auto child_filtered = tree_keep_if(f, child);
        children_filtered_grandchhildren.push_back(child_filtered);
    }
    result.children_ = children_filtered_grandchhildren;
    return result;
}

template <typename T>
std::string show_tree(
    const tree<T> &v,
    const tree_separators & separators,
    bool add_new_lines = true,
    const std::string &indent = "  ",
    int level = 0)
{
    auto line_start = fp::repeat(level, indent);

    std::string result;
    if (add_new_lines)
        result = line_start;

    result += fp::show(v.value_);

    if (!v.children_.empty())
    {
        if (add_new_lines)
            result += "\n" + line_start;
        result += separators.open_child;
        if (add_new_lines)
            result += "\n";

        std::vector<std::string> children_strs =
            fp_incompat::transform_vector<std::string>([=](const tree<T> &vv) -> std::string {
                return show_tree(vv, separators, add_new_lines, indent, level + 1);
            },
            v.children_);
        std::string children_str = join(show(separators.separate_siblings), children_strs);

        result += children_str;

        if (add_new_lines)
            result += "\n" + line_start;
        result += separators.close_child;
    }
    return result;
}

} // namespace fp_add
} // namespace fp
