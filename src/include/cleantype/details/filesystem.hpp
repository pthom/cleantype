// filesystem polyfills for C++14 (70's style)
#pragma once
#include <cleantype/details/cleantype_fp/fp_base.hpp>
#include <cleantype/details/stringutils.hpp>
#include <cstdlib>
#include <fstream>
#include <sys/stat.h>
#if defined(__unix__) || defined(__APPLE__)
#include <sys/types.h>
#include <unistd.h>
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
            char * answer = _getcwd(NULL, 0);
#else
#ifdef __APPLE__
            char buffer[PATH_MAX];
#else
            char buffer[MAXPATHLEN];
#endif
            char * answer = ::getcwd(buffer, sizeof(buffer));
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

        inline bool file_exists(const std::string & filename)
        {
            struct stat buffer;
            return (stat(filename.c_str(), &buffer) == 0);
        }

        inline std::string read_istream(std::istream & is)
        {
            std::stringstream ss;
            std::string line;
            while (!is.eof())
            {
                std::getline(is, line);
                ss << line << "\n";
            }
            return ss.str();
        }

        inline std::string read_file(const std::string & filename)
        {
            std::ifstream is(filename);
            return read_istream(is);
        }
    }  // namespace filesystem
}