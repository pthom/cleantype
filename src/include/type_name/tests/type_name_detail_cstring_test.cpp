#include "doctest.h"
#include <type_name/details/hana_utils/experimental/detail/type_name_pretty_function.hpp>
#include <type_name/details/hana_utils/experimental/detail/type_name_format.hpp>

#include <cstdlib>
#include <cstring>
#include <string>

#include <iostream>
#define LOG_VALUE(var) std::cout << #var << " ==>" << var << "<==" << std::endl

namespace hana = boost::hana;


// namespace boost {
// namespace hana {
// namespace experimental {

//     namespace cstring_utils {


namespace cstring_utils = boost::hana::experimental::cstring_utils;

// constexpr bool my_cstring_equal(cstring_utils::cstring const & cs1, cstring_utils::cstring const & cs2) {
//     //constexpr std::size_t l1 = cs1.length;
//     // if (cs1.length != cs2.length)
//     //     return false;

//     // std::size_t idx = 0;
//     // do {
//     //     char c1 = cs1.ptr[idx];
//     //     char c2 = cs2.ptr[idx];
//     //     if (c1 != c2)
//     //         return false;
//     //     idx++;
//     // } while (idx < cs1.length);
//     return true;
// }

using namespace cstring_utils;

constexpr bool my_cstring_equal(cstring const & cs1, cstring const & cs2) {
    if (cs1.length != cs2.length)
        return false;

    std::size_t idx = 0;
    do {
        char c1 = cs1.ptr[idx];
        char c2 = cs2.ptr[idx];
        if (c1 != c2)
            return false;
        idx++;
    } while (idx < cs1.length);
    return true;
}

void truc()
{
    constexpr cstring_utils::cstring cs1{"", 0}, cs2 {"", 0};
    constexpr bool b = my_cstring_equal(cs1, cs2);
}



void assert_cstring_equal_compiletime(const cstring_utils::cstring & computed, const cstring_utils::cstring & expected) {
    //constexpr bool test = my_cstring_equal(computed, expected);
    //constexpr std::size_t l1 = computed.length;

        //cstring_utils::cstring_equal(computed, expected);
    //static_assert( cstring_utils::cstring_equal(computed, expected), "STATIC_ASSERT_CSTRING_EQUAL error");
}



void assert_cstring_equal_runtime(const cstring_utils::cstring & computed, const cstring_utils::cstring & expected) {
    //bool cmp = cstring_utils::cstring_equal(computed, expected);
    // if (!cmp) {
    //     LOG_VALUE(cstring_utils::cstring_to_string(computed));
    //     LOG_VALUE(cstring_utils::cstring_to_string(expected));
    // }
    // REQUIRE(cmp);
}


#ifdef _HANA_TN_CAN_CONSTEXPR
    #define RUN_ONE_TYPE_TEST_COMPILE_TIME(type_definition)                               \
        {                                                                                 \
            constexpr auto computed_type_definition =                                     \
                hana::experimental::cstring_utils::type_name_impl_cstring<type_definition>();    \
            assert_cstring_equal_compiletime(computed_type_definition, #type_definition); \
        }
#else
    #define RUN_ONE_TYPE_TEST_COMPILE_TIME(type_definition)
#endif

#define RUN_ONE_TYPE_TEST_RUN_TIME(type_definition)                                      \
    {                                                                                    \
        auto computed_type_definition =                                                  \
            hana::experimental::cstring_utils::type_name_impl_cstring<type_definition>();\
        auto formatted =                                                                 \
            hana::experimental::type_name_format::format_type(computed_type_definition); \
        assert_cstring_equal_runtime(formatted, #type_definition);                       \
    }


#define RUN_ONE_TYPE_TEST_COMPILE_AND_RUN_TIME(type_definition)                       \
    RUN_ONE_TYPE_TEST_COMPILE_TIME(type_definition)                                   \
    RUN_ONE_TYPE_TEST_RUN_TIME(type_definition)


#define COMMA ,

template<typename... T>
struct Holder {
};

//////////// Actual tests Below

TEST_CASE("type_name_detail_cstring_test_compile_time") {
    // RUN_ONE_TYPE_TEST_COMPILE_AND_RUN_TIME(char);
    // RUN_ONE_TYPE_TEST_COMPILE_AND_RUN_TIME(char &);
    // RUN_ONE_TYPE_TEST_COMPILE_AND_RUN_TIME(char &&);
    // RUN_ONE_TYPE_TEST_COMPILE_AND_RUN_TIME(char *);
}

TEST_CASE("type_name_detail_cstring_test_run_time") {
    // Those test might fail at compile time because of different code conventions across compilers
    // RUN_ONE_TYPE_TEST_RUN_TIME(char *const);  // or "char * const"
    // RUN_ONE_TYPE_TEST_RUN_TIME(const char *); // or "const char*"

    //hana::experimental::detail::type_name_impl_cstring<Holder<char>>();
    //RUN_ONE_TYPE_TEST_RUN_TIME(Holder<char COMMA char>);
}
