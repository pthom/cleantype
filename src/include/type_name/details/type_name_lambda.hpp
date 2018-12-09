#pragma once
#include <type_name/details/type_name_clean.hpp>

namespace type_name
{
    namespace internal
    {
        inline std::string remove_outer_parenthesis(const std::string & s)
        {
            assert(s.size() >= 2 );
            assert(s.front() =='(');
            assert(s.back() ==')');
            std::string result(s.begin() + 1, s.end() - 1);
            return result;
        }


        struct extract_parenthesis_content_at_end_result
        {
            std::string remaining_at_start;
            std::string parenthesis_content;
            bool success;

            static extract_parenthesis_content_at_end_result error() {
                extract_parenthesis_content_at_end_result r;
                r.success = false;
                return r;
            }
        };


        // Returns { remaining_at_start = "ABC", parenthesis_content="GHI", success = true }
        inline extract_parenthesis_content_at_end_result extract_parenthesis_content_at_end(const std::string & str)
        {
            // Example :
            //  "ABC(DEF)(GHI)KLM"
            if (str.empty())
            {
                std::cerr << "extract_parenthesis_content_at_end_result : error (empty input)" << std::endl;
                return extract_parenthesis_content_at_end_result::error();
            }

            extract_parenthesis_content_at_end_result result;

            std::string s = str;

            while (s.back() != ')')
            {
                s.pop_back();
                if (s.empty())
                {
                    std::cerr << "extract_parenthesis_content_at_end_result: error (missing last ')' )" << std::endl;
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
                    nb_parenthesis ++;
                if (c == '(')
                    nb_parenthesis --;
                s.pop_back();
                if (s.empty())
                {
                    std::cerr << "extract_parenthesis_content_at_end_result: error (non zero count of '()' )" << std::endl;
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


        inline std::vector<std::string> tokenize_lambda_params(const std::string & params, bool clean_params)
        {
            auto clean_param_if_needed = [&clean_params](const std::string & param) {
                return clean_params ? impl_clean(param) : fp::trim(' ', param);
            };

            std::vector<std::string> result;
            // counts < and > occurrences
            int count = 0;
            std::string current;
            for (const auto c : params)
            {
                if (c == '<')
                    ++count;
                if (c == '>')
                    --count;
                if ( (c == ',') && (count == 0))
                {
                    result.push_back(clean_param_if_needed(current));
                    current = "";
                }
                else
                {
                    current += c;
                }
            }
            result.push_back(clean_param_if_needed(current));
            return result;
        }


        inline std::string _remove_mem_fn_surround(const std::string & mem_fn_type)
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


        inline std::string _mem_fn_to_lambda_type(const std::string & mem_fn_type, bool clean_params)
        {
            const std::string lambda_full_type = _remove_mem_fn_surround(mem_fn_type);
            // std::cout << lambda_full_type << std::endl;
            std::string params_str, return_str_with_leading_garbage;
            {
                // lambda params are at the end between parenthesis
                auto params_r = extract_parenthesis_content_at_end(lambda_full_type);
                if (!params_r.success)
                    std::cerr << "_mem_fn_to_lambda_type : error parsing mem_fn_type --> " << mem_fn_type << std::endl;
                params_str = params_r.parenthesis_content;
                return_str_with_leading_garbage = params_r.remaining_at_start;
            }

            // Separate params and clean them, then join them
            const std::string params_cleaned = [&](){
                auto params_list = tokenize_lambda_params(params_str, clean_params);
                std::string params_joined = fp::join(std::string(", "), params_list);
                if (params_joined == "void")
                  params_joined = "";
                return params_joined;
            }();

            // garbage between the parentheses before (lambda anonymous name)
            std::string return_str;
            {
                auto garbage_r = extract_parenthesis_content_at_end(return_str_with_leading_garbage);
                if (!garbage_r.success)
                    std::cerr << "_mem_fn_to_lambda_type : error parsing mem_fn_type --> " << mem_fn_type << std::endl;
                return_str = garbage_r.remaining_at_start;
            }

            std::string return_type = clean_params ? impl_clean(return_str) : return_str;
            // std::cout << "params= " << params << '\n';
            // std::cout << "return_type= " << return_type << '\n';
            return std::string("lambda: ") + "(" + params_cleaned + ")" + " -> " + return_type;
        }


        template <typename LambdaFunction>
        std::string type_lambda(LambdaFunction fn, bool clean_params)
        {
            // Examples of possible inputs:

            // auto f = [&c](int a, int b) -> double { return a + b + c; };
            // MSVC : class std::_Mem_fn<double (__thiscall <lambda_1d102738ade82cc35233c841173ca72c>::*)(int,int)const >
            // clang: std::__1::__mem_fn<double (type_name::_DOCTEST_ANON_FUNC_2()::$_1::*)(int, int) const>
            // MSVC : double (__thiscall <lambda_1d102738ade82cc35233c841173ca72c>::*)(int,int)const

            //auto f = [](int a, int b)  { return std::pair<int, int>(a, b); };
            //clang: std::__1::__mem_fn<std::__1::pair<int, int> (type_name::_DOCTEST_ANON_FUNC_2()::$_1::*)(int, int) const>
            //clang: std::__1::pair<int, int> (type_name::_DOCTEST_ANON_FUNC_2()::$_1::*)(int, int) const

            //clang: std::__1::pair<int, int> (type_name::_DOCTEST_ANON_FUNC_2()::$_1::*)(int, int) const

            auto as_mem_fn = std::mem_fn( & decltype(fn)::operator() );
            std::string mem_fn_type = tn_type_name_full(as_mem_fn);
            return _mem_fn_to_lambda_type(mem_fn_type, clean_params);
        }

    } // namespace internal


    template <typename LambdaFunction>
    std::string lambda_full(LambdaFunction fn)
    {
        return internal::type_lambda(fn, false);
    }

    template <typename LambdaFunction>
    std::string lambda_clean(LambdaFunction fn)
    {
        return internal::type_lambda(fn, true);
    }


    //////////////////////////////
    // Start of public API
    //////////////////////////////

    // * `type_name::lambda_clean<LambdaFn>(LambdaFn fn)` is a function that will return a string containing
    //    the readable signature of a lambda (non generic)
    template <typename LambdaFn> std::string lambda_clean(LambdaFn fn);
    // * `type_name::lambda_clean<LambdaFn>(LambdaFn fn)` is a function that will return a string containing
    //    the full signature of a lambda (non generic)
    template <typename LambdaFn> std::string lambda_clean(LambdaFn fn);

} // namespace type_name

#define tn_show_details_lambda(f) std::string("[") + type_name::lambda_clean(f) + "] " + #f
#define tn_show_details_lambda_full(f) std::string("[") + type_name::lambda_full(f) + "] " + #f
