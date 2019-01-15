// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)

#pragma once
#include <array>
#include <vector>
#include <string>
#include <map>
#include <fstream>


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

    //
    // In order to configure the behavior of clean types :
    // Create a file ~/.cleantype.json with the content of the string CleanConfigurationExample
    // (just above). Edit this file as a normal json file (you can use https://jsoneditoronline.org/
    // if you want an interactive editor)
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


#include <nlohmann/json.hpp>
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


#include <cstdlib>
#include <sys/stat.h>

#if defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif


namespace cleantype
{
    namespace filesystem
    {
        inline std::string safe_getenv(const std::string k)
        {
            char * p = getenv(k.c_str());
            if (p == NULL)
                return "";
            else
                return std::string(p);
        }

        inline std::string user_home_directory()
        {
            // cf https://stackoverflow.com/questions/2552416/how-can-i-find-the-users-home-dir-in-a-cross-platform-manner-using-c
            // Windows: env USERPROFILE or if this fails, concatenate HOMEDRIVE+HOMEPATH
            // Linux, Unix and OS X: env HOME or if this fails, use getpwuid() (example code)
            std::string home_directory;

            #if defined(__unix__) || defined(__APPLE__)
                struct passwd* pwd = getpwuid(getuid());
                if (pwd)
                    home_directory = pwd->pw_dir;
                else
                    home_directory = getenv("HOME");
            #else
                home_directory= safe_getenv("USERPROFILE");
                if (home_directory.empty())
                    home_directory= safe_getenv("HOMEDRIVE") + safe_getenv("HOMEPATH");
            #endif

            if (home_directory.empty())
            {
                home_directory = "./";
            }
            return home_directory;
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
            cleantype::CleanConfiguration gCleanConfiguration;
            std::string pref_file = cleantype::filesystem::user_home_directory() + ".cleantype.json";
            if (cleantype::filesystem::file_exists(pref_file))
            {
                std::ifstream is(pref_file);
                nlohmann::json json_data;
                is >> json_data;
                gCleanConfiguration = json_data.get<cleantype::CleanConfiguration>();
            }
            return gCleanConfiguration;
        }
    }

} // namespace cleantype
