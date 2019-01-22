// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once
#include <cassert>
#include <cleantype/cleantype_configuration.hpp>
#include <cleantype/details/cleantype_clean_impl.hpp>
#include <iostream>

namespace cleantype
{
    namespace internal
    {
        inline std::string remove_outer_parenthesis(std::string const & s)
        {
            assert(s.size() >= 2);
            assert(s.front() == '(');
            assert(s.back() == ')');
            std::string result(s.begin() + 1, s.end() - 1);
            return result;
        }

        struct extract_parenthesis_content_at_end_result
        {
            std::string remaining_at_start;
            std::string parenthesis_content;
            bool success;

            static extract_parenthesis_content_at_end_result error()
            {
                extract_parenthesis_content_at_end_result r;
                r.success = false;
                return r;
            }
        };

        // Example :
        //  "ABC(DEF)(GHI)KLM"
        // Returns { remaining_at_start = "ABC", parenthesis_content="GHI", success = true }
        inline extract_parenthesis_content_at_end_result extract_parenthesis_content_at_end(
            std::string const & str)
        {
            if (str.empty())
            {
                std::cerr << "extract_parenthesis_content_at_end_result : error (empty input)"
                          << std::endl;
                return extract_parenthesis_content_at_end_result::error();
            }

            extract_parenthesis_content_at_end_result result;

            std::string s = str;

            while (s.back() != ')')
            {
                s.pop_back();
                if (s.empty())
                {
                    std::cerr
                        << "extract_parenthesis_content_at_end_result: error (missing last ')' )"
                        << std::endl;
                    return extract_parenthesis_content_at_end_result::error();
                }
            }

            std::vector<char> content;
            int nb_parenthesis = 1;
            content.push_back(')');
            s.pop_back();
            while (nb_parenthesis > 0)
            {
                char c = s.back();
                content.push_back(c);
                if (c == ')')
                    nb_parenthesis++;
                if (c == '(')
                    nb_parenthesis--;
                s.pop_back();
                if (s.empty())
                {
                    std::cerr << "extract_parenthesis_content_at_end_result: error (non zero count "
                                 "of '()' )"
                              << std::endl;
                    return extract_parenthesis_content_at_end_result::error();
                }
            }
            std::reverse(content.begin(), content.end());
            for (auto c : content)
                result.parenthesis_content += c;

            result.parenthesis_content = remove_outer_parenthesis(result.parenthesis_content);
            result.remaining_at_start = s;
            result.success = true;
            return result;
        }

        inline std::string _remove_mem_fn_surround(std::string const & mem_fn_type)
        {
            std::string result = mem_fn_type;
            // Suppress mem_fn< at the start
            size_t idx1 = result.find('<');
            if (idx1 == std::string::npos)
                return "Error, can not find first '<' in mem_fn_type: " + mem_fn_type;
            result = result.substr(idx1 + 1);

            // Suppress all after the last ')'
            size_t idx2 = result.rfind(')');
            if (idx1 == std::string::npos)
                return "Error, can not find last '>' in mem_fn_type: " + mem_fn_type;
            result = result.substr(0, idx2 + 1);

            return result;
        }

        inline std::string _mem_fn_to_lambda_type(std::string const & mem_fn_type,
                                                  bool clean_params)
        {
            // Examples of possible inputs:

            // auto f = [&c](int a, int b) -> double { return a + b + c; };
            // MSVC : class std::_Mem_fn<double (__thiscall
            // <lambda_1d102738ade82cc35233c841173ca72c>::*)(int,int)const > clang:
            // std::__1::__mem_fn<double (cleantype::_DOCTEST_ANON_FUNC_2()::$_1::*)(int, int)
            // const> MSVC : double (__thiscall
            // <lambda_1d102738ade82cc35233c841173ca72c>::*)(int,int)const

            // auto f = [](int a, int b)  { return std::pair<int, int>(a, b); };
            // clang: std::__1::__mem_fn<std::__1::pair<int, int>
            // (cleantype::_DOCTEST_ANON_FUNC_2()::$_1::*)(int, int) const> clang:
            // std::__1::pair<int, int> (cleantype::_DOCTEST_ANON_FUNC_2()::$_1::*)(int, int) const

            const std::string lambda_full_type = _remove_mem_fn_surround(mem_fn_type);
            // std::cout << lambda_full_type << std::endl;
            std::string params_str, return_str_with_leading_garbage;
            {
                // lambda params are at the end between parenthesis
                auto params_r = extract_parenthesis_content_at_end(lambda_full_type);
                if (!params_r.success)
                    std::cerr << "_mem_fn_to_lambda_type : error parsing mem_fn_type --> "
                              << mem_fn_type << std::endl;
                params_str = params_r.parenthesis_content;
                return_str_with_leading_garbage = params_r.remaining_at_start;
            }

            // Separate params and clean them, then join them
            const std::string params_cleaned = [&]() {
                auto params_list = split_types(params_str);
                if (clean_params)
                    params_list = cleantype_fp::transform(impl_clean, params_list);
                std::string params_joined = cleantype_fp::join(std::string(", "), params_list);
                if (params_joined == "void")
                    params_joined = "";
                if (cleantype::CleanConfiguration::GlobalConfig().force_east_const_)
                    params_joined = cleantype::apply_east_const_typelist(params_joined);
                return params_joined;
            }();

            // garbage between the parentheses before (lambda anonymous name)
            std::string return_str;
            {
                auto garbage_r =
                    extract_parenthesis_content_at_end(return_str_with_leading_garbage);
                if (!garbage_r.success)
                    std::cerr << "_mem_fn_to_lambda_type : error parsing mem_fn_type --> "
                              << mem_fn_type << std::endl;
                return_str = garbage_r.remaining_at_start;
            }

            std::string return_type = clean_params ? impl_clean(return_str) : return_str;
            if (cleantype::CleanConfiguration::GlobalConfig().force_east_const_)
                return_type = cleantype::apply_east_const_typelist(return_type);

            // std::cout << "params= " << params << '\n';
            // std::cout << "return_type= " << return_type << '\n';
            return std::string("lambda: ") + "(" + params_cleaned + ")" + " -> " + return_type;
        }

    }  // namespace internal

}  // namespace cleantype
