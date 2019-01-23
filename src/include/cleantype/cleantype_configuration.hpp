// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)

#pragma once
#include <array>
#include <fstream>
#include <map>
#include <string>
#include <vector>

// In order to configure the behavior of clean types :
//
// * Duplicate the file .cleantype.json at the root of the cleantype repository,
//   and save this file in anywhere in the hierarchy of the parent directories
//   of the execution directory.
// * Edit this file as a normal json file (you can use also use an online editor like
//   https://jsoneditoronline.org/)
// * #define CLEANTYPE_USE_NLOHMANN_JSON before including this file and
//   add the include path to nlohmann/json.hpp)

namespace cleantype
{
    struct CleanConfiguration;
    namespace internal
    {
        CleanConfiguration ImplGlobalConfig();
    }  // namespace internal

    struct CleanConfiguration
    {
        std::vector<std::string> suppress_extra_namespaces_ = {"::__1", "::__cxx11"};

        // unnecessary class, struct given by msvc
        std::vector<std::string> suppress_extract_struct_class_ = {"class ", "struct "};

        // add your custom suppressions here
        std::vector<std::string> suppress_custom_ = {" __ptr64"};

        // these node are suppressed after parse of the template tree of a type
        std::vector<std::string> undesirable_type_nodes_ = {
            "std::char_traits", "std::allocator", "std::less"};

        std::map<std::string, std::string> replacements_after_undesirable_node_extractions = {
            {"std::basic_string<char>", "std::string"},
            {"basic_string<_CharT, _Traits, _Allocator>", "std::string"}};

        // set this to true in order to force the results to be east const
        // this is experimental, and can fail
        bool force_east_const_ = false;

        // * any type with a depth <= indent_depth_limit
        //   will be presented on a single line
        // * any type with a depth > indent_depth_limit
        //   will be presented on several indented lines
        // set  indent_depth_limit = 0 to disable indentation
        std::size_t indent_depth_limit = 3;

        inline static CleanConfiguration & GlobalConfig()
        {
            thread_local CleanConfiguration gCleanConfiguration = internal::ImplGlobalConfig();
            return gCleanConfiguration;
        }

        inline static CleanConfiguration _EmptyConfig()
        {
            CleanConfiguration r;
            r.suppress_extra_namespaces_ = {};
            r.suppress_extract_struct_class_ = {};
            r.suppress_custom_ = {};
            r.undesirable_type_nodes_ = {};
            r.replacements_after_undesirable_node_extractions = {};
            r.force_east_const_ = false;
            r.indent_depth_limit = 0;
            return r;
        }
    };

    inline bool force_east_const() { return CleanConfiguration::GlobalConfig().force_east_const_; }
    inline void set_force_east_const(bool v)
    {
        CleanConfiguration::GlobalConfig().force_east_const_ = v;
    }

    inline std::size_t indent_depth_limit()
    {
        return CleanConfiguration::GlobalConfig().indent_depth_limit;
    }
    inline void set_indent_depth_limit(std::size_t v)
    {
        CleanConfiguration::GlobalConfig().indent_depth_limit = v;
    }

}  // namespace cleantype

#include <cleantype/details/cleantype_configuration_json.hpp>
