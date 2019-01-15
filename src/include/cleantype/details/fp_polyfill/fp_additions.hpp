// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <deque>
#include <vector>
#include <algorithm>
#include <functional>

#include <cleantype/details/fp_polyfill/fp_polyfill.hpp>
#include <cleantype/details/fp_polyfill/fp_incompat.hpp>


// Additions to fplus
namespace fp
{
namespace fp_add
{

template<typename T>
std::vector<T> take_at_idxs(const std::vector<T> & xs, const std::vector<size_t> & indexes)
{
    std::vector<T> r;
    for (auto idx : indexes)
        r.push_back(xs[idx]);
    return r;
}

struct lhs_rhs
{
    std::string lhs;
    std::string rhs;
};

using lhs_rhs_tree = tree<lhs_rhs>;

struct tree_separators {
    char open_child = '<';
    char close_child = '>';
    char siblings_separator = ',';
};

enum class siblings_spacing_t
{
    no_spacing,
    space,
    new_line
};

struct show_tree_options
{
    bool add_new_lines_before_children = true;
    siblings_spacing_t siblings_spacing = siblings_spacing_t::no_spacing;
    std::string indent = "  ";
};

struct show_tree_lhs_rhs_options
{
    bool add_new_lines_before_children = true;
    bool add_space_after_lhs = false;
    bool add_space_before_rhs = false;
    siblings_spacing_t siblings_spacing = siblings_spacing_t::no_spacing;
    std::string indent = "  ";
};


inline lhs_rhs_tree parse_lhs_rhs_tree(
    std::string const &s,
    const tree_separators & separators,
    bool remove_space_after_separator
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

    bool is_first_after_separator = false;
    while (!fifo_letters.empty())
    {
        char c = fifo_letters.front();

        fifo_letters.pop_front();
        if (isspace(c))
        {
            if (remove_space_after_separator && is_first_after_separator)
                continue;
        }
        if (c == separators.open_child)
        {
            lhs_rhs_tree child{ {},{} };
            current->children_.push_back(child);
            parents.push_back(current);
            current = &(parents.back()->children_.back());
            is_first_after_separator = true;
            is_filling_rhs = false;
        }
        else if (c == separators.siblings_separator)
        {
            lhs_rhs_tree brother{ {},{} };
            parents.back()->children_.push_back(brother);
            current = &(parents.back()->children_.back());
            is_first_after_separator = true;
            is_filling_rhs = false;
        }
        else if (c == separators.close_child)
        {
            current = parents.back();
            parents.pop_back();
            is_first_after_separator = true;
            is_filling_rhs = true;
        }
        else
        {
            if (is_filling_rhs)
                current->value_.rhs += c;
            else
                current->value_.lhs += c;
            is_first_after_separator = false;
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

namespace detail {
    template <typename T>
    std::size_t tree_depth_impl(const tree<T> &xs, int current_depth) {
        std::vector<std::size_t> sizes;
        sizes.push_back(current_depth);
        for (auto & child : xs.children_)
            sizes.push_back(tree_depth_impl(child, current_depth + 1)  );
        return fp::maximum(sizes);
    }
}

template <typename T>
std::size_t tree_depth(const tree<T> &xs)
{
    return detail::tree_depth_impl(xs, 0);
}


template<typename T>
std::string show_tree_children(const std::vector<tree<T>> & children,
    const tree_separators & separators,
    const show_tree_lhs_rhs_options & show_tree_lhs_rhs_options_,
    int level = 0)
{
    std::vector<std::string> children_strs =
            fp_incompat::transform_vector<std::string>([=](const tree<T> &vv) -> std::string {
            return show_tree_lhs_rhs(vv, separators, show_tree_lhs_rhs_options_, level + 1);
        },
        children);

    const std::string siblings_separator = [&] {
        std::string separator_string(1, separators.siblings_separator);
        if (show_tree_lhs_rhs_options_.siblings_spacing == siblings_spacing_t::no_spacing)
            return separator_string;
        else if (show_tree_lhs_rhs_options_.siblings_spacing == siblings_spacing_t::space)
            return separator_string + " ";
        else { // if (show_tree_lhs_rhs_options_.siblings_spacing == siblings_spacing_t::new_line)
            return separator_string + std::string("\n");
        }
    }();

    std::string children_str = join(siblings_separator, children_strs);
    return children_str;
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
    if (show_tree_lhs_rhs_options_.add_new_lines_before_children)
        result = line_start;

    result += fp::show(v.value_.lhs);
    if (!v.value_.lhs.empty() && show_tree_lhs_rhs_options_.add_space_after_lhs)
        result += " ";

    if (!v.children_.empty())
    {
        if (show_tree_lhs_rhs_options_.add_new_lines_before_children)
            result += "\n" + line_start;
        result += separators.open_child;
        if (show_tree_lhs_rhs_options_.add_new_lines_before_children)
            result += "\n";

        std::string children_str = show_tree_children(v.children_, separators, show_tree_lhs_rhs_options_, level);

        result += children_str;

        if (show_tree_lhs_rhs_options_.add_new_lines_before_children)
            result += "\n" + line_start;
        result += separators.close_child;
    }

    if (!v.value_.rhs.empty() && show_tree_lhs_rhs_options_.add_space_before_rhs)
        result += " ";
    result += fp::show(v.value_.rhs);

    return result;
}

} // namespace fp_add
} // namespace fp
