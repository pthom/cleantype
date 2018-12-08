#include "doctest.h"
#include "type_name/type_name_clean.hpp"
#include "type_name/details/lambda_typename.hpp"
#include <fplus/fplus.hpp>
#include <functional>
#include <map>

#define LOG(str) std::cout << str << std::endl


////////////// Internal tests below

TEST_CASE("tokenize_lambda_params")
{
    using namespace type_name::internal;
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

