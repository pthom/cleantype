#pragma once
#include <string>
#include <deque>
#include "type_name/details/type_name_full.hpp"
#include "type_name/details/type_name_tree.hpp"
#include "type_name/details/fp_polyfill/fp_additions.hpp"

namespace type_name
{

    template <class T>
    std::string clean()
    {
        return internal::impl_clean( internal::impl_full<T>() );
    }
    template <typename First, typename Second, typename ...Rest>
    std::string clean () {
        return internal::impl_clean( internal::impl_full<First>() ) + ", " + clean<Second, Rest...>();
    }


    template <typename T> std::string clean(T && v) {
        return internal::impl_clean( internal::impl_full<T>() );
    }
    template <typename First, typename Second, typename ...Rest>
    std::string clean(First && first, Second && second, Rest... rest) {
        return internal::impl_clean( internal::impl_full<First>() ) + ", " + clean<Second, Rest...>(
            std::forward<Second>(second),
            std::forward<Rest>(rest)...
        );
    }


    template <class T> std::string show_details(T && v) {
        return clean<T>() + " = " + fp::show(v);
    }

} // namespace type_name


#define TN_type_name_clean(var) type_name::clean<decltype(var)>()


#define TN_show_details(var) \
        std::string("[") + TN_type_name_clean(var) + "] " + #var \
        + " = " \
        + fp::show(var)


#define TN_show_details_cont(var) \
        std::string("[") + TN_type_name_clean(var) + "] " + #var \
        + " = " \
        + fp::show_cont(var)
