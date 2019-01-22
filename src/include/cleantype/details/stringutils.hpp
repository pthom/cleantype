// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)

#pragma once

#include <cassert>
#include <string>
#include <vector>

namespace cleantype
{
    namespace stringutils
    {
        inline std::string remove_spaces_before(const char token, std::string const & str)
        {
            std::string result;
            bool space_before = false;
            for (auto c : str)
            {
                if ((c == token) && space_before)
                {
                    result.pop_back();
                }
                result = result + c;

                if (c == ' ')
                    space_before = true;
                else
                    space_before = false;
            }
            return result;
        }

        inline std::string remove_spaces_after(const char token, std::string const & str)
        {
            std::string result;
            bool token_before = false;
            for (auto c : str)
            {
                result = result + c;
                if ((c == ' ') && token_before)
                    result.pop_back();

                if (c == token)
                    token_before = true;
                else
                    token_before = false;
            }
            return result;
        }

        inline std::string remove_spaces_before_after(const char token, std::string const & str)
        {
            std::string result;
            result = remove_spaces_before(token, remove_spaces_after(token, str));
            return result;
        }

        inline std::string insert_spaces_before(const char token, std::string const & str)
        {
            std::string result;
            bool space_or_same_token_before = true;
            for (auto c : str)
            {
                if ((c == token) && !(space_or_same_token_before))
                    result = result + " ";
                result = result + c;
                if ((c == ' ') || (c == token))
                    space_or_same_token_before = true;
                else
                    space_or_same_token_before = false;
            }
            return result;
        }

        inline std::string insert_spaces_after(const char token, std::string const & str)
        {
            std::string result;
            bool token_before = false;
            for (auto c : str)
            {
                if (token_before && (c != ' '))
                    result = result + ' ';
                result = result + c;
                token_before = (c == token);
            }
            return result;
        }

        inline std::string insert_spaces_before_after(const char token, std::string const & str)
        {
            std::string result = insert_spaces_before(token, str);
            result = insert_spaces_after(token, result);
            return result;
        }

        inline std::string replace_tokens(std::string const & from,
                                          const std::string & to,
                                          std::string const & xs)
        {
            if (from.empty())
                return xs;
            std::string out = xs;
            std::size_t start_pos = 0;
            while ((start_pos = out.find(from, start_pos)) != std::string::npos)
            {
                out.replace(start_pos, from.length(), to);
                start_pos +=
                    to.length();  // In case 'to' contains 'from', like replacing 'x' with 'yx'
            }
            return out;
        }

        inline bool ends_with(std::string const & value, std::string const & ending)
        {
            if (ending.size() > value.size())
                return false;
            return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
        }

        inline bool starts_with(std::string const & value, std::string const & start)
        {
            return (value.find(start) == 0);
        }

        inline bool starts_ends_with(std::string const & value,
                                     std::string const & start,
                                     std::string const & end)
        {
            return (starts_with(value, start) && ends_with(value, end));
        }

        inline std::string remove_end(std::string const & value, std::string const & ending)
        {
            assert(ends_with(value, ending));
            std::string r = value.substr(0, value.size() - ending.size());
            return r;
        }

        inline std::string remove_start(std::string const & value, std::string const & start)
        {
            assert(starts_with(value, start));
            std::string r = value.substr(start.size(), value.size() - start.size());
            return r;
        }

        inline std::string remove_start_end(std::string const & value,
                                            std::string const & start,
                                            std::string const & end)
        {
            std::string r = remove_start(value, start);
            r = remove_end(r, end);
            return r;
        }

        inline std::vector<std::string> split_string(std::string const & s, char delimiter)
        {
            std::vector<std::string> tokens;
            std::string token;
            std::istringstream tokenStream(s);
            while (std::getline(tokenStream, token, delimiter))
                tokens.push_back(token);
            return tokens;
        }

    }  // namespace stringutils
}  // namespace cleantype
