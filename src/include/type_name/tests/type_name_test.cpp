#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "type_name/type_name_clean.hpp"
#include <fplus/fplus.hpp>
#include <functional>
#include <map>



TEST_CASE("log_var")
{
    {
        int a = 1;
        REQUIRE_EQ(log_var_str(a), "[int] a = 1");
    }
    {
        std::string s("hello");
        REQUIRE_EQ(log_var_str(s), "[std::string] s = hello");
    }
    {
        std::vector<int> v{{ 1, 2, 3, 4, 5}};
        REQUIRE_EQ(log_var_str(v), "[std::vector<int>] v = [1, 2, 3, 4, 5]");
    }
    {
        std::map<std::string, int> v {{
            {"a", 1},
            {"b", 2},
            {"c", 3}
        }};
         REQUIRE_EQ(log_var_str_cont(v), "[std::map<std::string, int>] v = [(a, 1), (b, 2), (c, 3)]");
    }
    {
        std::vector<std::string> v {{
            {"a"},
            {"b"},
            {"c"}
        }};
         REQUIRE_EQ(log_var_str(v), "[std::vector<std::string>] v = [a, b, c]");
    }
    {
        std::set<std::string> v {{
            {"a"},
            {"b"},
            {"c"}
        }};
         REQUIRE_EQ(log_var_str(v), "[std::set<std::string>] v = [a, b, c]");
    }
}


namespace type_name
{

template<typename T> void TestDefaultConstructibleType(const std::string & expectedTypeClean)
{
    const T v;
    std::string type_full = var_type_name_full(v);
    std::string type_clean = type_name::demangle_typename(type_full);
    std::cout << "type_full : " << type_full << "\n";
    std::cout << "type_clean: " << type_clean << '\n';
    std::cout << "expected  : " << expectedTypeClean << '\n';
    CHECK(type_clean == expectedTypeClean);
}

std::string remove_outer_parenthesis(const std::string & s)
{
    assert(s.size() >= 2 );
    assert(s.front() =='(');
    assert(s.back() ==')');
    std::string result(s.begin() + 1, s.end() - 1);
    return result;
}

struct extract_parenthesis_content_at_end_result
{
    std::string remaining;
    std::string parenthesis_content;
    bool success;
};

extract_parenthesis_content_at_end_result extract_parenthesis_content_at_end(const std::string & str)
{
    extract_parenthesis_content_at_end_result result;

    extract_parenthesis_content_at_end_result error; error.success = false;

    std::string s = str;

    while (s.back() != ')')
    {
        s.pop_back();
        if (s.empty())
            return error;
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
            return error;
    }
    std::reverse(content.begin(), content.end());
    for (auto c : content)
        result.parenthesis_content += c;
    result.remaining = s;
    result.success = true;
    return result;
}
// http://www.cplusplus.com/forum/general/223816/ pour wrapper une lambda dans une std::function
// marchera pas avec lambda polymorphique...


std::vector<std::string> tokenize_lambda_params(const std::string & params, bool clean)
{
    auto clean_param_if_needed = [&clean](const std::string & param) {
        return clean ? demangle_typename(param) : fp::trim(' ', param);
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

TEST_CASE("tokenize_lambda_params")
{
    {
        std::string params_str("int, string");
        auto params = tokenize_lambda_params(params_str, false);
        std::vector<std::string> expected {{
            {"int"},
            {"string"}
        }};
        REQUIRE_EQ(params, expected);
    }
    {
        std::string params_str("int, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, double");
        auto params = tokenize_lambda_params(params_str, false);
        std::vector<std::string> expected {{
            {"int"},
            {"std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >"},
            {"double"},
        }};
        REQUIRE_EQ(params, expected);
    }
    {
        std::string params_str("int, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, double");
        auto params = tokenize_lambda_params(params_str, true);
        std::vector<std::string> expected {{
            {"int"},
            {"std::string"},
            {"double"},
        }};
        REQUIRE_EQ(params, expected);
    }
    {
        std::string params_str("");
        auto params = tokenize_lambda_params(params_str, true);
        std::vector<std::string> expected {{
        }};
        REQUIRE_EQ(params, expected);
    }
    {
        std::string params_str("int");
        auto params = tokenize_lambda_params(params_str, true);
        std::vector<std::string> expected {{
            "int"
        }};
        REQUIRE_EQ(params, expected);
    }
}


std::string _mem_fn_to_lambda_type(const std::string & mem_fn_type, bool clean)
{
    std::string lambda_full_type = mem_fn_type;
    // Suppress mem_fn< at the start
    size_t idx1 = lambda_full_type.find('<');
    if (idx1 ==  std::string::npos)
        return "Error, can not find first '<' in lambda_full_type: " + lambda_full_type;
    lambda_full_type = lambda_full_type.substr(idx1 + 1);

    // Suppress all after the last ')'
    size_t idx2 = lambda_full_type.rfind(')');
    if (idx1 ==  std::string::npos)
        return "Error, can not find last '>' in lambda_full_type: " + lambda_full_type;
    lambda_full_type = lambda_full_type.substr(0, idx2 + 1);

    // lambda params are at the end between parenthesis
    auto params_r = extract_parenthesis_content_at_end(lambda_full_type);
    std::string params = remove_outer_parenthesis(params_r.parenthesis_content);

    // Separate params and clean them, then join them
    const std::string params_cleaned = [&](){
        auto params_list = tokenize_lambda_params(params, clean);
        return fp::join(std::string(" "), params_list);
    }();

    // garbage between the parentheses before (lambda anonymous name)
    auto garbage_r = extract_parenthesis_content_at_end(params_r.remaining);

    std::string return_type = clean ? demangle_typename(garbage_r.remaining) : garbage_r.remaining;
    // std::cout << "params= " << params << '\n';
    // std::cout << "return_type= " << return_type << '\n';
    return std::string("lambda: ") + "(" + params_cleaned + ")" + " -> " + return_type;
}

template <typename LambdaFunction>
std::string type_lambda(LambdaFunction fn, bool clean)
{
    // auto f = [&c](int a, int b) -> double { return a + b + c; };
    // MSVC : class std::_Mem_fn<double (__thiscall <lambda_1d102738ade82cc35233c841173ca72c>::*)(int,int)const >
    // clang: std::__1::__mem_fn<double (type_name::_DOCTEST_ANON_FUNC_2()::$_1::*)(int, int) const>
    // MSVC : double (__thiscall <lambda_1d102738ade82cc35233c841173ca72c>::*)(int,int)const

    //auto f = [](int a, int b)  { return std::pair<int, int>(a, b); };
    //clang: std::__1::__mem_fn<std::__1::pair<int, int> (type_name::_DOCTEST_ANON_FUNC_2()::$_1::*)(int, int) const>
    //clang: std::__1::pair<int, int> (type_name::_DOCTEST_ANON_FUNC_2()::$_1::*)(int, int) const

    //clang: std::__1::pair<int, int> (type_name::_DOCTEST_ANON_FUNC_2()::$_1::*)(int, int) const

    // algo :
    // 1.
    // au debut : aller jusqu'au premier < (et le supprimer)
    // a la fin : aller jusqu'a la premiere ")" (et la garder)
    //
    //
    // 2. Aller jusqu'a la premiere parenthese, tout

    auto as_mem_fn = std::mem_fn( & decltype(fn)::operator() );
     // ajouter un param template ici
    // auto as_mem_fn = std::mem_fn( & decltype(fn)::operator<Args...>() );

    std::string mem_fn_type = var_type_name_full(as_mem_fn);
    return _mem_fn_to_lambda_type(mem_fn_type, clean);
}

template <typename LambdaFunction>
std::string type_lambda_full(LambdaFunction fn)
{
    return type_lambda(fn, false);
}

template <typename LambdaFunction>
std::string type_lambda_clean(LambdaFunction fn)
{
    return type_lambda(fn, true);
}


#define log_type_lambda_full_str(f) std::string("[") + type_lambda_full(f) + "] " + #f + " = "
#define log_type_lambda_clean_str(f) std::string("[") + type_lambda_clean(f) + "] " + #f + " = "
#define log_type_lambda_full(f) std::cout << log_type_lambda_full_str(f) << std::endl;
#define log_type_lambda_clean(f) std::cout << log_type_lambda_clean_str(f) << std::endl;


template <typename LambdaFunction, typename... Args>
std::string type_lambda_variadic(LambdaFunction fn, bool clean)
{
    auto as_mem_fn = std::mem_fn( & LambdaFunction::template operator()<Args...> );
    std::string mem_fn_type = var_type_name_full(as_mem_fn);
    return _mem_fn_to_lambda_type(mem_fn_type, clean);
}


// std::string make_type_lambda_variadic()
auto make_type_lambda_variadic = [](auto f) {

};

// TODO :
// - type of polymorphic lambdas
// - type of functions (wrap in std::function ?)


TEST_CASE("testing sample_lib")
{
    // int c = 5;
    // auto f = [&c](int a, int b) -> double { return a + b + c; };
    //auto f = [](int a, int b)  { return std::pair<int, int>(a, b); };

    // std::basic_string <char> s_;
    // std::basic_string <char> &s(s_);
    // std::cout << var_name_type_name_clean(s) << "\n";

    // auto f = [](const int & a, int & b)  { return std::pair<int, double>(a + b, cos(a + b)); };
    // std::cout << log_type_lambda(f) << "\n";

    // auto f2 = []()  { return 42; };
    // std::cout << log_type_lambda(f2) << "\n";

    // auto f3 = []()  { std::cout << "Hello"; };
    // std::cout << log_type_lambda(f3) << "\n";

    // auto f4 = []()  { return std::string("Hello"); };
    // std::cout << log_type_lambda(f4) << "\n";

    int a = 5;
    log_var(a);

    auto f5 = [](const std::string & s)  { return s + s; };
    log_type_lambda_clean(f5);

    // auto my_square = [](int a) { return a * a;};
    // auto my_double = [](int a) { return a * 2; };
    // auto ff = fplus::fwd::compose(my_square, my_double);

    // std::cout << '\n';
    // std::cout << type_lambda_variadic<decltype(ff), int>(ff) << std::endl;



    //std::cout << type_lambda(ff) << "\n";

    // auto t = std::mem_fn( & decltype(f)::operator() );
    // std::string type_full = var_type_name_full(
    //     t );
    // std::cout << "type_full : " << type_full << "\n";

    // TestDefaultConstructibleType
    //     <
    //         const std::string
    //     >
    //     (
    //         "const std::string"
    //     );
    // TestDefaultConstructibleType
    //     <
    //         const std::vector<int>
    //     >
    //     (
    //         "const std::vector<int>"
    //     );
    // TestDefaultConstructibleType
    //     <
    //         const std::deque<unsigned long>
    //     >
    //     (
    //         "const std::deque<unsigned long>"
    //     );
    // TestDefaultConstructibleType
    //     <
    //         const std::vector<std::pair<std::string, int>>
    //     >
    //     (
    //         "const std::vector<std::pair<std::string, int>>"
    //     );



    // {
    //     const std::vector<std::pair<std::string, int>> v;
    //     //const auto &&vv = std::move(v);
    //     std::string type_full = var_type_name_full(v);
    //     std::string type_clean = type_name::demangle_typename(type_full);
    //     std::cout << "type_full: " << type_full << "\n";
    //     std::cout << "type_clean: " << type_clean << '\n';
    // }
    // {
    //     std::cout << "\n";
    //     const std::vector<std::pair<std::string, int>> v;
    //     const auto &&vv = std::move(v);
    //     auto s = var_type_name_full(vv);
    //     std::cout << "Mangled:\n"
    //               << s << "\n";
    //     std::cout << "\n";
    //     std::cout << "Demangled:\n"
    //               << type_name::demangle_typename(s) << '\n';
    // }

    // {
    //     std::cout << "\n";
    //     auto f = [](const std::vector<std::string> &v, int a) {
    //         return v.size() + a;
    //     };
    //     auto s = var_type_name_full(f);
    //     std::cout << "Mangled:\n"
    //               << s << "\n";
    //     std::cout << "\n";
    //     std::cout << "Demangled:\n"
    //               << type_name::demangle_typename(s) << '\n';
    // }
}
}
