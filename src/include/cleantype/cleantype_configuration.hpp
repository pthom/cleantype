// This file is part of ConsType : Consistent Type names for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)

#pragma once
#include <array>
#include <vector>
#include <string>

namespace cleantype
{

    //
    // In order to configure the behavior of clean types, either :
    // - edit this file before compilation
    // - modify CleanConfiguration::GlobalConfig()
    //
    struct CleanConfiguration
    {
        std::vector<std::string> suppress_extra_namespaces_ = {
            "::__1",
            "::__cxx11"
        };

        // unnecessary class, struct given by msvc
        std::vector<std::string> suppress_extract_struct_class_ = {
            "class ",
            "struct "
        };

        // add your custom suppressions here
        std::vector<std::string> suppress_custom_ = {
            " __ptr64"
        };

        // these node are suppressed after parse of the template tree of a type
        std::vector<std::string> undesirable_type_nodes_ = {
            "std::char_traits",
            "std::allocator",
            "std::less"
        };

        using StringReplace = std::array<std::string, 2>;
        std::vector<StringReplace> replacements_after_undesirable_node_extractions =
        {
            { "std::basic_string<char>", "std::string" },
            { "basic_string<_CharT, _Traits, _Allocator>", "std::string"}
        };

        // set this to true in order to force the results to be east const
        // this is experimental, and can fail
        bool force_east_const_ = false;

        static CleanConfiguration & GlobalConfig()
        {
            thread_local CleanConfiguration gCleanConfiguration;
            return gCleanConfiguration;
        }
    };


}