// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once

// These functions parse and manipulate trees in a functional way (this is the basis
// of the type parsing in this project)

#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <cleantype/details/cleantype_fp/fp_base.hpp>
#include <cleantype/details/cleantype_fp/fp_show.hpp>

namespace cleantype_fp_tree
{
    template <typename T>
    struct tree
    {
        tree(const T & value, const std::vector<tree<T>> & children)
            : value_(value), children_(children)
        {
        }
        T value_;
        std::vector<tree<T>> children_;
    };

    struct lhs_rhs
    {
        std::string lhs;
        std::string rhs;
    };

    using lhs_rhs_tree = tree<lhs_rhs>;

    struct tree_separators
    {
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

    enum class children_indent_t
    {
        no_indent,
        newline_before_open_child,
        newline_before_child
    };

    struct show_tree_lhs_rhs_options
    {
        children_indent_t children_indent;
        bool add_space_after_lhs = false;
        bool add_space_before_rhs = false;
        siblings_spacing_t siblings_spacing = siblings_spacing_t::no_spacing;
        std::string indent = "  ";
    };

    inline lhs_rhs_tree parse_lhs_rhs_tree(std::string const & s,
                                           const tree_separators & separators,
                                           bool remove_space_after_separator)
    {
        bool is_filling_rhs = false;

        std::deque<char> fifo_letters;
        {
            for (const auto & letter : s)
                fifo_letters.push_back(letter);
        }

        lhs_rhs_tree result{{}, {}};
        lhs_rhs_tree * current = &result;
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
                lhs_rhs_tree child{{}, {}};
                current->children_.push_back(child);
                parents.push_back(current);
                current = &(parents.back()->children_.back());
                is_first_after_separator = true;
                is_filling_rhs = false;
            }
            else if (c == separators.siblings_separator)
            {
                lhs_rhs_tree brother{{}, {}};
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
    tree<T> tree_keep_if(std::function<bool(const T &)> f, const tree<T> & xs)
    {
        tree<T> result = xs;

        result.children_ = cleantype_fp::keep_if([f](const tree<T> & xxs) { return f(xxs.value_); },
                                                 result.children_);

        std::vector<tree<T>> children_filtered_grandchhildren;
        for (auto & child : result.children_)
        {
            auto child_filtered = tree_keep_if(f, child);
            children_filtered_grandchhildren.push_back(child_filtered);
        }
        result.children_ = children_filtered_grandchhildren;
        return result;
    }

    template <typename T, typename Transformer_T>
    void tree_transform_leafs_depth_first_inplace(Transformer_T transformer, tree<T> & xs_io)
    {
        for (auto & child : xs_io.children_)
            tree_transform_leafs_depth_first_inplace(transformer, child);
        transformer(xs_io.value_);
    }

    template <typename T, typename Transformer_T>
    void tree_transform_leafs_breadth_first_inplace(Transformer_T transformer, tree<T> & xs_io)
    {
        transformer(xs_io.value_);
        for (auto & child : xs_io.children_)
            tree_transform_leafs_breadth_first_inplace(transformer, child);
    }

    namespace detail
    {
        template <typename T>
        std::size_t tree_depth_impl(const tree<T> & xs, int current_depth)
        {
            std::vector<std::size_t> sizes;
            sizes.push_back(current_depth);
            for (auto & child : xs.children_)
                sizes.push_back(tree_depth_impl(child, current_depth + 1));
            return cleantype_fp::maximum(sizes);
        }
    }  // namespace detail

    template <typename T>
    std::size_t tree_depth(const tree<T> & xs)
    {
        return detail::tree_depth_impl(xs, 0);
    }

    template <typename OutputType, typename InputType, typename F>
    std::vector<OutputType> transform_vector(F f, const std::vector<InputType> & xs)
    {
        std::vector<OutputType> out;
        for (const auto & v : xs)
        {
            out.push_back(f(v));
        }
        return out;
    }

    template <typename T>
    std::string show_tree_children(const std::vector<tree<T>> & children,
                                   const tree_separators & separators,
                                   const show_tree_lhs_rhs_options & options,
                                   int level = 0)
    {
        std::vector<std::string> children_strs = transform_vector<std::string>(
            [=](const tree<T> & vv) -> std::string {
                return show_tree_lhs_rhs(vv, separators, options, level + 1);
            },
            children);

        const std::string siblings_separator = [&] {
            std::string separator_string(1, separators.siblings_separator);
            if (options.siblings_spacing == siblings_spacing_t::no_spacing)
                return separator_string;
            else if (options.siblings_spacing == siblings_spacing_t::space)
                return separator_string + " ";
            else
            {  // if (options.siblings_spacing == siblings_spacing_t::new_line)
                return separator_string + std::string("\n");
            }
        }();

        std::string children_str = cleantype_fp::join(siblings_separator, children_strs);
        return children_str;
    }

    template <typename T>
    std::string show_tree_lhs_rhs(const tree<T> & v,
                                  const tree_separators & separators,
                                  const show_tree_lhs_rhs_options & options,
                                  int level = 0)
    {
        auto line_start = cleantype_fp::repeat(level, options.indent);

        std::string result;
        if (options.children_indent != cleantype_fp_tree::children_indent_t::no_indent)
            result = line_start;

        result += cleantype_fp::show(v.value_.lhs);
        if (!v.value_.lhs.empty() && options.add_space_after_lhs)
            result += " ";

        if (!v.children_.empty())
        {
            if (options.children_indent == children_indent_t::newline_before_open_child)
                result += "\n" + line_start;
            result += separators.open_child;
            if (options.children_indent != children_indent_t::no_indent)
                result += "\n";

            std::string children_str = show_tree_children(v.children_, separators, options, level);

            result += children_str;

            if (options.children_indent != children_indent_t::no_indent)
                result += "\n" + line_start;
            result += separators.close_child;
        }

        if (!v.value_.rhs.empty() && options.add_space_before_rhs)
            result += " ";
        result += cleantype_fp::show(v.value_.rhs);

        return result;
    }

}  // namespace cleantype_fp_tree
