// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)

#pragma once
#include <array>
#include <vector>
#include <string>
#include <map>
#include <fstream>

// in order to read preferences from the settings file (.cleantype.json),
// #define CLEANTYPE_USE_NLOHMANN_JSON before including this file
// (and add the include path to nlohmann/json.hpp)

#ifdef CLEANTYPE_USE_NLOHMANN_JSON
#include <nlohmann/json.hpp>
#endif

namespace cleantype
{
    struct CleanConfiguration;
    namespace internal
    {
        std::string user_home_directory();
        bool file_exists (const std::string& name);
        CleanConfiguration ImplGlobalConfig();
    }

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




    // In order to configure the behavior of clean types :
    // * Create a file ".cleantype.json" with the content of the string CleanConfigurationExample (just above)
    //   (or duplicate the file .cleantype.json at the root of the cleantype repository)
    // * Edit this file as a normal json file (you can use also use an online editor like https://jsoneditoronline.org/)
    // * Save this file in anywhere in the hierarchy of the parent directories of the execution directory.
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

        std::map<std::string, std::string> replacements_after_undesirable_node_extractions =
        {
            { "std::basic_string<char>", "std::string" },
            { "basic_string<_CharT, _Traits, _Allocator>", "std::string"}
        };

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
            r.replacements_after_undesirable_node_extractions  = {};
            r.force_east_const_ = false;
            r.indent_depth_limit = 0;
            return r;
        }
    };
}

#ifndef CLEANTYPE_USE_NLOHMANN_JSON
namespace cleantype {
    namespace internal {
        inline CleanConfiguration ImplGlobalConfig() { return cleantype::CleanConfiguration(); }
    }
}

#else
namespace cleantype
{
    inline void to_json(nlohmann::json& j, const cleantype::CleanConfiguration & c)
    {
        j = nlohmann::json
        {
            {"suppress_extra_namespaces_", c.suppress_extra_namespaces_},
            {"suppress_extract_struct_class_", c.suppress_extract_struct_class_},
            {"suppress_custom_", c.suppress_custom_},
            {"undesirable_type_nodes_", c.undesirable_type_nodes_},
            {"replacements_after_undesirable_node_extractions", c.replacements_after_undesirable_node_extractions},
            {"force_east_const_", c.force_east_const_},
            {"indent_depth_limit", c.indent_depth_limit}
        };
    }

    inline void from_json(const nlohmann::json& j, cleantype::CleanConfiguration& c)
    {
        c = cleantype::CleanConfiguration::_EmptyConfig();
        j.at("suppress_extra_namespaces_").get_to(c.suppress_extra_namespaces_);
        j.at("suppress_extract_struct_class_").get_to(c.suppress_extract_struct_class_);
        j.at("suppress_custom_").get_to(c.suppress_custom_);
        j.at("undesirable_type_nodes_").get_to(c.undesirable_type_nodes_);
        j.at("replacements_after_undesirable_node_extractions").get_to(c.replacements_after_undesirable_node_extractions);
        j.at("force_east_const_").get_to(c.force_east_const_);
        j.at("indent_depth_limit").get_to(c.indent_depth_limit);
    }

}


// filesystem polyfills for C++14 (70's style)
#include <cstdlib>
#include <sys/stat.h>
#include <cleantype/details/cleantype_fp/fp_base.hpp>
#include <cleantype/details/stringutils.hpp>
#if defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#include <sys/types.h>
#endif
#if defined(__unix__) || defined(__APPLE__)
#include <sys/param.h>
#endif
#ifdef _WIN32
#include <direct.h>
#endif


namespace cleantype
{
    namespace filesystem
    {
        inline std::string getcwd()
        {
            #ifdef _WIN32
            char *answer = _getcwd(NULL, 0);
            #else
            #ifdef __APPLE__
            char buffer[PATH_MAX];
            #else
            char buffer[MAXPATHLEN];
            #endif
            char *answer = ::getcwd(buffer, sizeof(buffer));
            #endif

            std::string s;
            if (answer)
                s = answer;
            return s;
        }

        inline std::vector<std::string> parent_directories()
        {
            std::string cwd = cleantype::filesystem::getcwd();
            cwd = cleantype_fp::replace_tokens(std::string("\\"), std::string("/"), cwd);

            std::vector<std::string> folder_elems = stringutils::split_string(cwd, '/');

            if (folder_elems.empty())
                return {};

            std::vector<std::string> parent_dirs;
            std::string current = "";
            for (const auto & folder_elem : folder_elems)
            {
                current = current + folder_elem + "/";
                parent_dirs.push_back(current);
            }

            parent_dirs = cleantype_fp::reverse(parent_dirs);
            return parent_dirs;
        }

        inline bool file_exists (const std::string& name) {
            struct stat buffer;
            return (stat (name.c_str(), &buffer) == 0);
        }
    }


    namespace internal
    {
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
                    cleantype::CleanConfiguration config = json_data.get<cleantype::CleanConfiguration>();
                    return config;
                }
            }
            return cleantype::CleanConfiguration();
        }
    }

} // namespace cleantype

#endif // #ifndef CLEANTYPE_USE_NLOHMANN_JSON