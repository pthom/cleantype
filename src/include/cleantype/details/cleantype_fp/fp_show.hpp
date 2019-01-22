// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)

// These functions are inspired by
// (FunctionalPlus)[https://github.com/Dobiasd/FunctionalPlus], a Functional
// Programming Library for C++, which I highly recommend.
//
// See its api search site (a la Haskell):
// http://www.editgym.com/fplus-api-search/ and see demo at
// https://code-ballads.net/generated-notebooks/cpp/repl_cling/markdown/#A-REPL-session-of-C++-functional-programming,-using-fplus

#pragma once

#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace cleantype_fp
{
    template <typename T>
    std::string show(const T & v)
    {
        std::stringstream ss;
        ss << v;
        return ss.str();
    }
    inline std::string show(const bool & v) { return v ? "true" : "false"; }
    inline std::string show(const char * xs) { return std::string(xs); }

    template <typename T>
    std::string show_cont(T & xs);

}  // namespace cleantype_fp

// Below : overload for common containers
// (tuple is not yet included)

#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace cleantype_fp
{
    template <typename T, std::size_t N>
    std::string show(const std::array<T, N> & v)
    {
        return show_cont(v);
    }
    template <typename T>
    std::string show(const std::vector<T> & v)
    {
        return show_cont(v);
    }
    template <typename T>
    std::string show(const std::deque<T> & v)
    {
        return show_cont(v);
    }
    template <typename T, typename U>
    std::string show(const std::map<T, U> & v)
    {
        return show_cont(v);
    }

    template <typename T>
    std::string show(const std::forward_list<T> & v)
    {
        return cleantype_fp::show_cont(v);
    }
    template <typename T>
    std::string show(const std::list<T> & v)
    {
        return cleantype_fp::show_cont(v);
    }
    template <typename T>
    std::string show(const std::stack<T> & v)
    {
        return cleantype_fp::show_cont(v);
    }
    template <typename T>
    std::string show(const std::queue<T> & v)
    {
        return cleantype_fp::show_cont(v);
    }
    template <typename T>
    std::string show(const std::priority_queue<T> & v)
    {
        return cleantype_fp::show_cont(v);
    }

    template <typename T>
    std::string show(const std::set<T> & v)
    {
        return cleantype_fp::show_cont(v);
    }
    template <typename T>
    std::string show(const std::multiset<T> & v)
    {
        return cleantype_fp::show_cont(v);
    }

    template <typename T, typename U>
    std::string show(const std::multimap<T, U> & v)
    {
        return cleantype_fp::show_cont(v);
    }

    template <typename T>
    std::string show(const std::unordered_set<T> & v)
    {
        return cleantype_fp::show_cont(v);
    }
    template <typename T>
    std::string show(const std::unordered_multiset<T> & v)
    {
        return cleantype_fp::show_cont(v);
    }
    template <typename T, typename U>
    std::string show(const std::unordered_map<T, U> & v)
    {
        return cleantype_fp::show_cont(v);
    }
    template <typename T, typename U>
    std::string show(const std::unordered_multimap<T, U> & v)
    {
        return cleantype_fp::show_cont(v);
    }

    template <typename T, typename U>
    std::string show(const std::pair<T, U> & xs)
    {
        return std::string("(") + show(xs.first) + ", " + show(xs.second) + ")";
    }

    template <typename T>
    std::string show_cont(T & xs)
    {
        std::vector<std::string> strs;
        for (const auto & x : xs)
            strs.push_back(cleantype_fp::show(x));

        return std::string("[") + cleantype_fp::join(", ", strs) + "]";
    }

}  // namespace cleantype_fp
