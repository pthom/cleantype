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
