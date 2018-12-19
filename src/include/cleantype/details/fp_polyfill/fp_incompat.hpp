// This file is part of ConsType : Consistent Type names for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once

#include <vector>

namespace fp_incompat
{
    // this is a simplified version of fplus::transform that accepts only vectors
    template <typename OutputType, typename InputType, typename F>
    std::vector<OutputType> transform_vector(F f, const std::vector<InputType>& xs) {
        std::vector<OutputType> out;
        for (const auto & v : xs) {
            out.push_back(f(v));
        }
        return out;
    }
} // namespace fp_incompat
