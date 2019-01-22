#include <cleantype/cleantype.hpp>
#include <fplus/fplus.hpp>
#include <iostream>

namespace types_variations
{
    using TypeString = std::string;
    using TypeStringList = std::vector<TypeString>;

    TypeStringList distinct_types(const TypeStringList & l)
    {
        using namespace std::string_literals;
        auto remove_double_spaces = [](const TypeString & T) {
            return fplus::replace_tokens("  "s, " "s, T);
        };
        TypeStringList l2 = fplus::transform(remove_double_spaces, l);
        return fplus::nub(l2);  // nub means "distinct"
    };

    namespace internal
    {
        TypeStringList space_variations_around_token_impl1(char token,
                                                           const TypeString & lhs,
                                                           const TypeString & rhs)
        {
            TypeStringList r = {lhs + " " + token + " " + rhs,
                                lhs + " " + token + rhs,
                                lhs + token + " " + rhs,
                                lhs + token + rhs};
            return r;
        }
        TypeStringList space_variations_around_token_impl2(char token,
                                                           const TypeStringList & lhs,
                                                           const TypeString & rhs)
        {
            auto apply_rhs = [&](const TypeString & T) {
                return space_variations_around_token_impl1(token, T, rhs);
            };
            std::vector<std::vector<TypeString>> ll = fplus::transform(apply_rhs, lhs);
            auto flattened = fplus::concat(ll);
            return flattened;
        }
    }  // namespace internal

    TypeStringList space_variations_around_token(char token, const TypeString & xs)
    {
        auto elems = fplus::split(token, true, xs);
        if (elems.size() == 1)
            return {elems.front()};
        auto fold_fn = [token](const TypeStringList & lhs, const TypeString & rhs) {
            return internal::space_variations_around_token_impl2(token, lhs, rhs);
        };
        TypeStringList init{elems.front()};
        auto r = fplus::fold_left(fold_fn, init, fplus::tail(elems));
        return distinct_types(r);
    };

    auto combine_transform1_then2 = [](auto && fn1, auto && fn2) {
        auto r = [&](const TypeString & typ) -> TypeStringList {
            TypeStringList l_1 = fn1(typ);
            std::vector<TypeStringList> ll_1_2 = fplus::transform(fn2, l_1);
            TypeStringList l_flattened = fplus::concat(ll_1_2);
            return l_flattened;
        };
        return r;
    };
    auto combine_transform1_then2_then3 = [](auto && fn1, auto && fn2, auto && fn3) {
        auto f12 = combine_transform1_then2(fn1, fn2);
        auto f = combine_transform1_then2(f12, fn3);
        return f;
    };
    auto combine_transform1_then2_then3_then4 =
        [](auto && fn1, auto && fn2, auto && fn3, auto && fn4) {
            auto f12 = combine_transform1_then2(fn1, fn2);
            auto f34 = combine_transform1_then2(fn3, fn4);
            auto f = combine_transform1_then2(f12, f34);
            return f;
        };

    TypeString remove_uneeded_spaces(const TypeString & xs)
    {
        using namespace std::string_literals;
        std::string r = xs;
        // r = fplus::replace_tokens(", "s, ","s, r);
        // r = fplus::replace_tokens(" ,"s, ","s, r);
        r = fplus::replace_tokens("  "s, " "s, r);
        r = fplus::replace_tokens("  "s, " "s, r);
        r = fplus::replace_tokens("  "s, " "s, r);
        r = fplus::replace_tokens("> "s, ">"s, r);
        r = fplus::replace_tokens(" >"s, ">"s, r);
        r = fplus::replace_tokens("< "s, "<"s, r);
        r = fplus::replace_tokens(" <"s, "<"s, r);
        r = fplus::replace_tokens("& "s, "&"s, r);
        r = fplus::replace_tokens(" &"s, "&"s, r);
        r = fplus::replace_tokens("* "s, "*"s, r);
        r = fplus::replace_tokens(" *"s, "*"s, r);
        return r;
    }

    auto make_space_variations_around_token = [](char token) {
        auto f = [token](const TypeString & xs) {
            return space_variations_around_token(token, xs);
        };
        return f;
    };

    TypeStringList space_variations(const TypeString & xs)
    {
        auto ptr_variations = [](const TypeString & xs) {
            return space_variations_around_token('*', xs);
        };
        auto ref_variations = [](const TypeString & xs) {
            return space_variations_around_token('&', xs);
        };
        auto opent_variations = [](const TypeString & xs) {
            return space_variations_around_token('<', xs);
        };
        auto closet_variations = [](const TypeString & xs) {
            return space_variations_around_token('>', xs);
        };
        auto all_spaces_fn = combine_transform1_then2_then3_then4(
            ptr_variations, ref_variations, opent_variations, closet_variations);
        return distinct_types(all_spaces_fn(remove_uneeded_spaces(xs)));
    }

    auto add_ptr = [](const TypeString & T) -> TypeStringList {
        using namespace std::string_literals;
        TypeStringList r = {T + "*"s};
        return r;
    };

    auto add_ref = [](const TypeString & T) -> TypeStringList {
        using namespace std::string_literals;
        TypeStringList r = {T + "&"s};
        return r;
    };

    auto add_const = [](const TypeString & T) -> TypeStringList {
        using namespace std::string_literals;
        TypeStringList r = {"const "s + T, T + " const"s};
        return r;
    };

    auto add_const_ptr = [](const TypeString & T) -> TypeStringList {
        using namespace std::string_literals;
        TypeStringList r = {"const "s + T + "*"s, T + " const "s + "*"s};
        return r;
    };

    auto add_const_ref = [](const TypeString & T) -> TypeStringList {
        using namespace std::string_literals;
        TypeStringList r = {"const "s + T + "&"s, T + " const "s + "&"s};
        return r;
    };

    auto add_const_ptr_const = [](const TypeString & T) -> TypeStringList {
        using namespace std::string_literals;
        TypeStringList r = {
            "const "s + T + " * const",
            T + " const " + " * const",
        };
        return r;
    };

    auto compiler_maybe_add_struct = [](const TypeString & T) -> TypeStringList {
        using namespace std::string_literals;
        TypeStringList r = {T, "struct "s + T};
        return r;
    };

    auto compiler_maybe_add_std_namespace = [](const TypeString & T) -> TypeStringList {
        using namespace std::string_literals;
        TypeStringList r = {T,
                            fplus::replace_tokens("std::"s, "std::__cxx11::"s, T),
                            fplus::replace_tokens("std::"s, "std::__1::"s, T),
                            T};
        return r;
    };

    auto compiler_maybe_add_ptr64 = [](const TypeString & T) -> TypeStringList {
        using namespace std::string_literals;
        TypeStringList r = {T + "*"s, T + "* __ptr64"s};
        return r;
    };

    auto add_vector = [](const TypeString & T) -> TypeStringList {
        using namespace std::string_literals;
        TypeStringList r = {"std::vector<"s + T + ">"s,
                            "std::vector<"s + T + ", std::allocator<" + T + ">>"s};
        return r;
    };

}  // namespace types_variations

template <typename T>
std::vector<T> take_n_samples(const std::vector<T> & xs, std::size_t n)
{
    assert(n != 0);
    if (n > xs.size())
        return xs;
    std::size_t step = xs.size() / n;
    std::vector<T> r;
    for (std::size_t i = 0; i < n; i++)
        r.push_back(xs[i * step]);
    return r;
}

std::string show_type_string_list_title(const types_variations::TypeStringList & l,
                                        int nb_samples_max)
{
    using namespace std::string_literals;
    std::string title =
        "  \""s + l.front() + "\""s + " - "s + fplus::show(l.size()) + " Variations  "s;
    if (l.size() > nb_samples_max)
        title = title + "\n\t" + fplus::show(nb_samples_max) + " samples out of " +
                fplus::show(l.size());
    std::string separator_line = fplus::repeat(title.size(), "-"s);
    return separator_line + "\n" + title + "\n" + separator_line + "\n";
}

std::string show_type_string_list(const types_variations::TypeStringList & l,
                                  int nb_samples_max = 10)
{
    std::stringstream r;
    auto samples = take_n_samples(l, nb_samples_max);
    r << show_type_string_list_title(l, nb_samples_max);
    for (const auto & x : samples)
        r << "\"" << x << "\""
          << "\n";
    r << "\n";
    return r.str();
}
