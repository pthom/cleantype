// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)

#pragma once
#include <functional>
#include <iostream>
#include <iterator>

namespace cleantype_fp_interact
{
    template <typename F>
    std::function<void()> interact(F f)
    {
        return [f]() -> void {
            std::cout << f(std::string(std::istreambuf_iterator<char>(std::cin.rdbuf()),
                                       std::istreambuf_iterator<char>()));
        };
    }

    template <typename F, typename InputStream, typename OutputStream>
    std::function<void()> interact_by_line(F f, InputStream & is, OutputStream & os)
    {
        return [f, &is, &os]() -> void {
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

}  // namespace cleantype_fp_interact
