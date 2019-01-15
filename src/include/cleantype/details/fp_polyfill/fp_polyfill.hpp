// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fplus/fplus.hpp> // REMOVE ME !!!

namespace fp
{
template <typename T>
struct tree
{
    tree (const T& value, const std::vector<tree<T>>& children) :
        value_(value), children_(children) {}
    T value_;
    std::vector<tree<T>> children_;
};

// this is a simplified version of fplus::repeat that accepts only string
inline std::string repeat(std::size_t n, const std::string& xs)
{
    std::string xss;
    for (std::size_t i = 0; i < n; i++)
        xss += xs;
    return xss;
}

// this is a simplified version of fplus::transform that accepts only vectors
template <typename F, typename T, typename U = T >
std::vector<U> transform(F f, const std::vector<T>& xs) {
    std::vector <U> out;
    for (const auto & x : xs)
        out.push_back(f(x));
    return out;
}


// this is a simplified version of fplus::keep_if that accepts only vectors
template <typename T, typename F>
std::vector<T> keep_if(F f, const std::vector<T>& xs) {
    std::vector<T> out;
    for (const auto & v : xs) {
        if (f(v))
            out.push_back(v);
    }
    return out;
}


// this is a simplified version of fplus::join that accepts only strings
inline std::string join(std::string const & separator, const std::vector<std::string> & xs)
{
    std::string out;
    for (size_t idx = 0; idx < xs.size(); idx++)
    {
        out += xs[idx];
        if ( idx < xs.size() - 1 )
            out+= separator;
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

// this is a simplified version of fplus::trim that accepts only vectors
template<typename T>
std::vector<T> trim(const T & trim_what, const std::vector<T> & xs)
{
    bool stop = false;
    std::vector<T> out;
    for (const auto & v : xs)
    {
        if (stop)
            out.push_back(v);
        else if (v != trim_what) {
            out.push_back(v);
            stop = true;
        }
    }
    while( (!out.empty()) && (out.back() == trim_what))
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
        else if (v != trim_what) {
            out.push_back(v);
            stop = true;
        }
    }
    while( (!out.empty()) && (out.back() == trim_what))
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

// TO REDO
template <typename T>
std::string show(const T & v)
{
    return fplus::show(v);
}

template <typename T>
std::string show_cont(T & v)
{
    return fplus::show_cont(v);
}

// template <typename T>
// std::string show(T & v)
// {
//     std::stringstream ss;
//     ss << v;
//     return ss.str();
// }

// template <typename T>
// std::string show(const std::vector<T> & xs)
// {
//     std::stringstream ss;
//     ss << "[";
//     bool has_previous = false;
//     for (const auto & x : xs)
//     {
//         if (has_previous)
//             ss << ", ";
//         //ss << x;
//         has_previous = true;
//     }
//     ss << "]";
//     return ss.str();
// }


// this is a simplified version of fplus::replace_tokens that accepts only strings
inline std::string replace_tokens(const std::string& from, const std::string& to, const std::string& xs)
{
    if(from.empty())
        return xs;
    std::string out = xs;
    size_t start_pos = 0;
    while((start_pos = out.find(from, start_pos)) != std::string::npos)
    {
        out.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
    return out;
}

template <typename F>
std::function<void()> interact(F f)
{
    return [f]() -> void
    {
        std::cout << f(std::string(
            std::istreambuf_iterator<char>(std::cin.rdbuf()),
            std::istreambuf_iterator<char>()));
    };
}


template <typename F, typename InputStream, typename OutputStream>
std::function<void()> interact_by_line(F f, InputStream & is, OutputStream & os)
{
    return [f, &is, &os]() -> void
    {
        std::string line;
        while (!is.eof())
        {
            std::getline(is, line);
            std::string out = f(line);
            os << out << "\n";
        }
    };
}

template <typename F>
std::function<void()> interact_by_line(F f)
{
    return interact_by_line(f, std::cin, std::cout);
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

template<typename T>
std::vector<T> numbers(const T start, const T end)
{
    std::vector<T> r;
    T v = start;
    while (v < end)
        r.push_back(v++);
    return r;
}

template<typename T>
std::vector<T> numbers(const T end)
{
    return numbers<T>( static_cast<T>(0), end);
}


template <typename T, typename U>
std::vector<std::pair<T, U>> zip(
    const std::vector<T> & xs, const std::vector<U> & ys)
{
    std::vector<std::pair<T, U>> r;

    auto len = std::min( xs.size(), ys.size() );
    for (std::size_t idx = 0; idx < len; idx++)
    {
        r.push_back( { xs[idx], ys[idx] } );
    }
    return r;
}


template <typename T, typename U>
std::pair<std::vector<T>, std::vector<U>> unzip(
    const std::vector<std::pair<T, U>> & xs)
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
T maximum(const std::vector<T>& xs)
{
    assert( ! xs.empty() );
    T max = xs[0];
    for (const auto & v: xs)
        if (v > max)
            max = v;
    return max;
}

}
