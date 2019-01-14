#include "doctest.h"
#include <cleantype/cleantype.hpp>

#define LOG(...) std::cout << __VA_ARGS__ << "\n";
#define LOG_VAR(...) std::cout << #__VA_ARGS__ << "\n" << __VA_ARGS__ << "\n";

template<typename... T> struct Foo {};

namespace wip_indent
{
        inline std::string code_pair_tree_to_string(cleantype::internal::code_pair_tree const & xs)
        {
            return fp::fp_add::show_tree_lhs_rhs(
                    xs,
                    cleantype::internal::make_template_tree_separators(),
                    cleantype::internal::make_template_show_tree_options_impl(true));
        }
}

TEST_CASE("indent")
{
    // {
    //     std::string s("ranges::v3::single_view<int, double>");
    //     cleantype::internal::code_pair_tree template_tree = cleantype::internal::parse_template_tree(s);
    //     auto s2 = code_pair_tree_to_string(template_tree);
    //     LOG_VAR(s2);
    // }
    // {
    //     std::string s("ranges::v3::detail::take_exactly_view_<ranges::v3::join_view<ranges::v3::transform_view<ranges::v3::iota_view<int, void>, (lambda at ../src/include/cleantype/tests/cleantype_rangev3_test.cpp:97:25)>, void>, false> &");
    //     cleantype::internal::code_pair_tree template_tree = cleantype::internal::parse_template_tree(s);
    //     auto s2 = wip_indent::code_pair_tree_to_string(template_tree);
    //     LOG_VAR(s2);
    // }

    {
        std::string s("Foo<int, char>");
        cleantype::internal::code_pair_tree template_tree = cleantype::internal::parse_template_tree(s);
        auto s2 = wip_indent::code_pair_tree_to_string(template_tree);
        LOG_VAR(s2);
    }
    {
        //using T = Foo< int, Foo<int, Foo<std::string, int>>>;
        using T = Foo<int, char>;
        LOG_VAR( cleantype::clean<T, std::string>() );
    }
}
