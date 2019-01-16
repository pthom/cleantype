// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)

#pragma once

#include <string>
#include <vector>
#include <cassert>

namespace cleantype
{
namespace stringutils
{
    inline bool ends_with(std::string const & value, std::string const & ending)
    {
        if (ending.size() > value.size())
            return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }


    inline bool starts_with(std::string const & value, std::string const & start)
    {
        return ( value.find(start) == 0);
    }


    inline bool starts_ends_with(std::string const & value, std::string const & start, std::string const & end)
    {
        return (starts_with(value, start) && ends_with(value, end));
    }


    inline std::string remove_end(std::string const & value, std::string const & ending)
    {
        assert(ends_with(value, ending));
        std::string r = value.substr(0, value.size() - ending.size());
        return r;
    }


    inline std::string remove_start(std::string const & value, std::string const & start)
    {
        assert(starts_with(value, start));
        std::string r = value.substr(start.size(), value.size() - start.size());
        return r;
    }


    inline std::string remove_start_end(std::string const & value, std::string const & start, std::string const & end)
    {
        std::string r = remove_start(value, start);
        r = remove_end(r, end);
        return r;
    }


    inline std::vector<std::string> split_string(std::string const & s, char delimiter)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter))
            tokens.push_back(token);
        return tokens;
    }

}
}
