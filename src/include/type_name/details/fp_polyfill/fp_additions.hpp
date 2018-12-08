#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <deque>
#include <vector>
#include <algorithm>
#include <functional>

#include "type_name/details/fp_polyfill/fp_polyfill.hpp"
#include "type_name/details/fp_polyfill/fp_incompat.hpp"


// Additions to fplus
namespace fp
{
namespace fp_add
{

struct lhs_rhs
{
    std::string lhs;
    std::string rhs;
};

using string_tree = tree<std::string>;
using lhs_rhs_tree = tree<lhs_rhs>;

struct tree_separators {
    char open_child = '<';
    char close_child = '>';
    char separate_siblings = ',';
};

struct show_tree_options
{
    bool add_new_lines = true;
    bool add_space_between_siblings = false;
    std::string indent = "  ";
};

struct show_tree_lhs_rhs_options
{
    bool add_new_lines = true;
    bool add_space_between_siblings = false;
    bool add_space_after_lhs = false;
    bool add_space_before_rhs = false;
    std::string indent = "  ";
};


inline lhs_rhs_tree parse_lhs_rhs_tree(
    const std::string &s,
    const tree_separators & separators,
    bool remove_space_after_tok,
    bool remove_space
    )
{
    bool is_filling_rhs = false;

    std::deque<char> fifo_letters;
    {
        for (const auto & letter : s)
            fifo_letters.push_back(letter);
    }

    lhs_rhs_tree result{ {},{} };
    lhs_rhs_tree *current = &result;
    std::vector<lhs_rhs_tree *> parents;

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
            lhs_rhs_tree child{ {},{} };
            current->children_.push_back(child);
            parents.push_back(current);
            current = &(parents.back()->children_.back());
            is_first_after_tok = true;
            is_filling_rhs = false;
        }
        else if (c == separators.separate_siblings)
        {
            lhs_rhs_tree brother{ {},{} };
            parents.back()->children_.push_back(brother);
            current = &(parents.back()->children_.back());
            is_first_after_tok = true;
            is_filling_rhs = false;
        }
        else if (c == separators.close_child)
        {
            current = parents.back();
            parents.pop_back();
            is_first_after_tok = true;
            is_filling_rhs = true;
        }
        else
        {
            if (is_filling_rhs)
                current->value_.rhs += c;
            else
                current->value_.lhs += c;
            is_first_after_tok = false;
        }
    }
    return result;
}



inline string_tree parse_string_tree(
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

    string_tree result{ {}, {} };
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
            string_tree child{ {}, {} };
            current->children_.push_back(child);
            parents.push_back(current);
            current = &(parents.back()->children_.back());
            is_first_after_tok = true;
        }
        else if (c == separators.separate_siblings)
        {
            string_tree brother{ {}, {} };
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


template <typename T, typename Transformer_T>
void tree_transform_leafs_depth_first_inplace(Transformer_T transformer, tree<T> &xs_io)
{
    for (auto & child : xs_io.children_)
        tree_transform_leafs_depth_first_inplace(transformer, child);
    transformer(xs_io.value_);
}

template <typename T, typename Transformer_T>
void tree_transform_leafs_breadth_first_inplace(Transformer_T transformer, tree<T> &xs_io)
{
    transformer(xs_io.value_);
    for (auto & child : xs_io.children_)
        tree_transform_leafs_breadth_first_inplace(transformer, child);
}


template <typename T>
std::string show_tree_lhs_rhs(
    const tree<T> &v,
    const tree_separators & separators,
    const show_tree_lhs_rhs_options & show_tree_lhs_rhs_options_,
    int level = 0)
{
    auto line_start = fp::repeat(level, show_tree_lhs_rhs_options_.indent);

    std::string result;
    if (show_tree_lhs_rhs_options_.add_new_lines)
        result = line_start;

    result += fp::show(v.value_.lhs);
    if (!v.value_.lhs.empty() && show_tree_lhs_rhs_options_.add_space_after_lhs)
        result += " ";

    if (!v.children_.empty())
    {
        if (show_tree_lhs_rhs_options_.add_new_lines)
            result += "\n" + line_start;
        result += separators.open_child;
        if (show_tree_lhs_rhs_options_.add_new_lines)
            result += "\n";

        std::vector<std::string> children_strs =
            fp_incompat::transform_vector<std::string>([=](const tree<T> &vv) -> std::string {
            return show_tree_lhs_rhs(vv, separators, show_tree_lhs_rhs_options_, level + 1);
        },
                v.children_);

        const std::string siblings_separator =
            show_tree_lhs_rhs_options_.add_space_between_siblings ?
            show(separators.separate_siblings) + " "
            : show(separators.separate_siblings);
        std::string children_str = join(siblings_separator, children_strs);

        result += children_str;

        if (show_tree_lhs_rhs_options_.add_new_lines)
            result += "\n" + line_start;
        result += separators.close_child;
    }

    if (!v.value_.rhs.empty() && show_tree_lhs_rhs_options_.add_space_before_rhs)
        result += " ";
    result += fp::show(v.value_.rhs);

    return result;
}


template <typename T>
std::string show_tree(
    const tree<T> &v,
    const tree_separators & separators,
    const show_tree_options & show_tree_options_,
    int level = 0)
{
    auto line_start = fp::repeat(level, show_tree_options_.indent);

    std::string result;
    if (show_tree_options_.add_new_lines)
        result = line_start;

    result += fp::show(v.value_);

    if (!v.children_.empty())
    {
        if (show_tree_options_.add_new_lines)
            result += "\n" + line_start;
        result += separators.open_child;
        if (show_tree_options_.add_new_lines)
            result += "\n";

        std::vector<std::string> children_strs =
            fp_incompat::transform_vector<std::string>([=](const tree<T> &vv) -> std::string {
                return show_tree(vv, separators, show_tree_options_, level + 1);
            },
            v.children_);

        const std::string siblings_separator =
            show_tree_options_.add_space_between_siblings ?
                    show(separators.separate_siblings) + " "
                :   show(separators.separate_siblings);
        std::string children_str = join(siblings_separator, children_strs);

        result += children_str;

        if (show_tree_options_.add_new_lines)
            result += "\n" + line_start;
        result += separators.close_child;
    }
    return result;
}

} // namespace fp_add
} // namespace fp
