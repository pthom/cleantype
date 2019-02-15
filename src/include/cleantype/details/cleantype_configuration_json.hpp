// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)

#pragma once
#include <cleantype/cleantype_configuration.hpp>
#include <cleantype/details/filesystem.hpp>

#ifndef CLEANTYPE_USE_NLOHMANN_JSON
namespace cleantype {
  namespace internal {
      inline CleanConfiguration ImplGlobalConfig() { return cleantype::CleanConfiguration(); }
  }
}
#else

#include <array>
#include <fstream>
#include <map>
#include <string>
#include <vector>


#include <nlohmann/json.hpp>

namespace cleantype
{

    inline void to_json(nlohmann::json & j, const cleantype::CleanConfiguration & c)
    {
        j = nlohmann::json{{"suppress_extra_namespaces_", c.suppress_extra_namespaces_},
                           {"suppress_extract_struct_class_", c.suppress_extract_struct_class_},
                           {"suppress_custom_", c.suppress_custom_},
                           {"undesirable_type_nodes_", c.undesirable_type_nodes_},
                           {"replacements_after_undesirable_node_extractions",
                            c.replacements_after_undesirable_node_extractions},
                           {"force_east_const_", c.force_east_const_},
                           {"indent_depth_limit", c.indent_depth_limit}};
    }

    inline void from_json(const nlohmann::json & j, cleantype::CleanConfiguration & c)
    {
        c = cleantype::CleanConfiguration::_EmptyConfig();
        j.at("suppress_extra_namespaces_").get_to(c.suppress_extra_namespaces_);
        j.at("suppress_extract_struct_class_").get_to(c.suppress_extract_struct_class_);
        j.at("suppress_custom_").get_to(c.suppress_custom_);
        j.at("undesirable_type_nodes_").get_to(c.undesirable_type_nodes_);
        j.at("replacements_after_undesirable_node_extractions")
            .get_to(c.replacements_after_undesirable_node_extractions);
        j.at("force_east_const_").get_to(c.force_east_const_);
        j.at("indent_depth_limit").get_to(c.indent_depth_limit);
    }

    namespace internal
    {
        static std::string CleanConfigurationExample = R"(
{
    "force_east_const_": true,
    "indent_depth_limit": 3,
    "replacements_after_undesirable_node_extractions": {
        "basic_string<_CharT, _Traits, _Allocator>": "std::string",
        "std::basic_string<char>": "std::string"
    },
    "suppress_custom_": [
        " __ptr64"
    ],
    "suppress_extra_namespaces_": [
        "::__1",
        "::__cxx11"
    ],
    "suppress_extract_struct_class_": [
        "class ",
        "struct "
    ],
    "undesirable_type_nodes_": [
        "std::char_traits",
        "std::allocator",
        "std::less"
    ]
}
    )";


        inline CleanConfiguration ImplGlobalConfig()
        {
            std::vector<std::string> parent_dirs = cleantype::filesystem::parent_directories();
            for (const std::string & dir : parent_dirs)
            {
                std::string pref_file = dir + "/.cleantype.json";
                if (cleantype::filesystem::file_exists(pref_file))
                {
                    std::ifstream is(pref_file);
                    nlohmann::json json_data;
                    is >> json_data;
                    cleantype::CleanConfiguration config =
                        json_data.get<cleantype::CleanConfiguration>();
                    return config;
                }
            }
            return cleantype::CleanConfiguration();
        }
    }  // namespace internal

}  // namespace cleantype

#endif  // #ifndef CLEANTYPE_USE_NLOHMANN_JSON
