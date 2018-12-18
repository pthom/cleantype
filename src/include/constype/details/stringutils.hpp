// This file is part of ConsType : Consistent Type names for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)

#pragma once

#include <string>
#include <vector>

namespace constype
{
namespace stringutils
{
    inline bool ends_with(const std::string & value, const std::string & ending)
    {
        if (ending.size() > value.size()) 
            return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }

    inline bool starts_with(const std::string & value, const std::string & start)
    {
        return ( value.find(start) == 0);
    }

    inline bool starts_ends_with(const std::string & value, const std::string & start, const std::string & end)
    {
        return (starts_with(value, start) && ends_with(value, end));
    }


    inline std::string remove_end(const std::string & value, const std::string & ending)
    {
        assert(ends_with(value, ending));
        std::string r = value.substr(0, value.size() - ending.size());
        return r;
    }

    inline std::string remove_start(const std::string & value, const std::string & start)
    {
        assert(starts_with(value, start));
        std::string r = value.substr(start.size(), value.size() - start.size());
        return r;
    }

    inline std::string remove_start_end(const std::string & value, const std::string & start, const std::string & end)
    {
        std::string r = remove_start(value, start);
        r = remove_end(r, end);
        return r;
    }
}
}
