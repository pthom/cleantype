
#pragma once

#include <string>
#include <vector>

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
std::string repeat(std::size_t n, const std::string& xs)
{
    std::string xss;
    for (std::size_t i = 0; i < n; i++)
        xss += xs;
    return xss;
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
std::string join(const std::string & separator, const std::vector<std::string> & xs)
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

// this is a simplified version of fplus::trim that accepts only vectors
template<typename T>
std::vector<T> trim(const T & trim_what, const std::vector<T> & xs)
{
    std::vector<T> out;
    for (const auto & v : xs)
    {
        if (v != trim_what)
            out.push_back(v);
    }
    return out;
}
std::string trim(const char & trim_what, const std::string & xs)
{
    std::string out;
    for (const auto & v : xs)
    {
        if (v != trim_what)
            out.push_back(v);
    }
    return out;
}


template <typename T>
std::string show(T && v)
{
    std::stringstream ss;
    ss << v;
    return ss.str();
}

// this is a simplified version of fplus::replace_tokens that accepts only strings
std::string replace_tokens(const std::string& from, const std::string& to, const std::string& xs)
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
}
