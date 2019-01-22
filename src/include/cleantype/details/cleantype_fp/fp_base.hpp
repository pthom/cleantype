// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)

// These functions are inspired by (FunctionalPlus)[https://github.com/Dobiasd/FunctionalPlus],
// a Functional Programming Library for C++, which I highly recommend.
// See fplus its api search site (a la Haskell): http://www.editgym.com/fplus-api-search/
// and see demo at
// https://code-ballads.net/generated-notebooks/cpp/repl_cling/markdown/#A-REPL-session-of-C++-functional-programming,-using-fplus

// Note: These versions are intentionally simplified and not completely generic.

#pragma once

#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace cleantype_fp
{
    inline std::string repeat(std::size_t n, const std::string & xs)
    {
        std::string xss;
        for (std::size_t i = 0; i < n; i++)
            xss += xs;
        return xss;
    }

    template <typename F, typename T, typename U = T>
    std::vector<U> transform(F f, const std::vector<T> & xs)
    {
        std::vector<U> out;
        for (const auto & x : xs)
            out.push_back(f(x));
        return out;
    }

    template <typename T, typename F>
    std::vector<T> keep_if(F f, const std::vector<T> & xs)
    {
        std::vector<T> out;
        for (const auto & v : xs)
        {
            if (f(v))
                out.push_back(v);
        }
        return out;
    }

    inline std::string join(std::string const & separator, const std::vector<std::string> & xs)
    {
        std::string out;
        for (size_t idx = 0; idx < xs.size(); idx++)
        {
            out += xs[idx];
            if (idx < xs.size() - 1)
                out += separator;
        }
        return out;
    }

    inline std::string join(std::string const & separator, const std::deque<std::string> & xs)
    {
        std::string out;
        for (size_t idx = 0; idx < xs.size(); idx++)
        {
            out += xs[idx];
            if (idx < xs.size() - 1)
                out += separator;
        }
        return out;
    }

    template <typename T>
    std::vector<T> trim(const T & trim_what, const std::vector<T> & xs)
    {
        bool stop = false;
        std::vector<T> out;
        for (const auto & v : xs)
        {
            if (stop)
                out.push_back(v);
            else if (v != trim_what)
            {
                out.push_back(v);
                stop = true;
            }
        }
        while ((!out.empty()) && (out.back() == trim_what))
            out.pop_back();
        return out;
    }

    inline std::string trim(const char & trim_what, std::string const & xs)
    {
        bool stop = false;
        std::string out;
        for (const auto & v : xs)
        {
            if (stop)
                out.push_back(v);
            else if (v != trim_what)
            {
                out.push_back(v);
                stop = true;
            }
        }
        while ((!out.empty()) && (out.back() == trim_what))
            out.pop_back();
        return out;
    }

    inline std::string trim_whitespace(std::string const & s)
    {
        std::string r = trim(' ', s);
        r = trim('\t', r);
        r = trim('\n', r);
        return r;
    }

    inline std::string replace_tokens(const std::string & from,
                                      const std::string & to,
                                      const std::string & xs)
    {
        if (from.empty())
            return xs;
        std::string out = xs;
        size_t start_pos = 0;
        while ((start_pos = out.find(from, start_pos)) != std::string::npos)
        {
            out.replace(start_pos, from.length(), to);
            start_pos += to.length();  // In case 'to' contains 'from', like replacing 'x' with 'yx'
        }
        return out;
    }

    template <typename T, typename Pred>
    std::vector<T> keep_if(Pred pred, std::vector<T> && xs)
    {
        std::vector<T> r;
        for (const auto & v : xs)
            if (pred(v))
                r.push_back(v);
        return r;
    }

    template <typename T>
    std::vector<T> numbers(const T start, const T end)
    {
        std::vector<T> r;
        T v = start;
        while (v < end)
            r.push_back(v++);
        return r;
    }

    template <typename T>
    std::vector<T> numbers(const T end)
    {
        return numbers<T>(static_cast<T>(0), end);
    }

    template <typename T, typename U>
    std::vector<std::pair<T, U>> zip(const std::vector<T> & xs, const std::vector<U> & ys)
    {
        std::vector<std::pair<T, U>> r;

        auto len = std::min(xs.size(), ys.size());
        for (std::size_t idx = 0; idx < len; idx++)
        {
            r.push_back({xs[idx], ys[idx]});
        }
        return r;
    }

    template <typename T, typename U>
    std::pair<std::vector<T>, std::vector<U>> unzip(const std::vector<std::pair<T, U>> & xs)
    {
        std::pair<std::vector<T>, std::vector<U>> r;
        for (const auto & v : xs)
        {
            r.first.push_back(v.first);
            r.second.push_back(v.second);
        }
        return r;
    }

    inline std::string to_upper_case(const std::string & xs)
    {
        std::string r = xs;
        std::transform(r.begin(), r.end(), r.begin(), ::toupper);
        return r;
    }

    inline std::string to_lower_case(const std::string & xs)
    {
        std::string r = xs;
        std::transform(r.begin(), r.end(), r.begin(), ::tolower);
        return r;
    }

    template <typename T>
    T maximum(const std::vector<T> & xs)
    {
        assert(!xs.empty());
        T max = xs[0];
        for (const auto & v : xs)
            if (v > max)
                max = v;
        return max;
    }

    template <typename T>
    std::vector<T> reverse(const std::vector<T> & xs)
    {
        std::vector<T> r;
        for (std::size_t i = xs.size(); i > 0; i--)
            r.push_back(xs[i - 1]);
        return r;
    }

}  // namespace cleantype_fp
