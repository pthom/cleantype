#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "type_name/type_name_clean.hpp"
#include <fplus/fplus.hpp>
#include <functional>


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
// http://www.cplusplus.com/forum/general/223816/ pour wrapper une lambda dans un std::function
// marchera pas avec lambda polymorphique...
// auto glambda = [] (auto a) { return a; };
// Will make glambda an instance of this type:

// class /* unnamed */
// {
// public:
//     template<typename T>
//     T operator () (T a) const { return a; }
// };
template <typename LambdaFunction>
std::string type_lambda(LambdaFunction fn)
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

    std::string lambda_full_type = var_type_name_full(as_mem_fn);

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
    std::string params = params_r.parenthesis_content;

    // garbage between the parentheses before (lambda anonymous name)
    auto garbage_r = extract_parenthesis_content_at_end(params_r.remaining);

    std::string return_type = garbage_r.remaining;

    // std::cout << "params= " << params << '\n';
    // std::cout << "return_type= " << return_type << '\n';
    return std::string("lambda: ") + params + " -> " + return_type;
}


TEST_CASE("testing sample_lib")
{
    // int c = 5;
    // auto f = [&c](int a, int b) -> double { return a + b + c; };
    //auto f = [](int a, int b)  { return std::pair<int, int>(a, b); };

    auto f = [](int & a, int & b)  { return std::pair<int, double>(a + b, cos(a + b)); };
    std::cout << type_lambda(f) << "\n";

    auto my_square = [](int a) { return a * a;};
    auto my_double = [](int a) { return a * 2; };
    auto ff = fplus::fwd::compose(my_square, my_double);
    auto as_mem_fn = std::mem_fn( & decltype(ff)::operator()<int> );
    std::cout << var_type_name_full(as_mem_fn) << '\n';

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
