// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once
#include <cleantype/details/cleantype_fp/fp_base.hpp>
#include <cleantype/details/stringutils.hpp>
#include <string>

namespace cleantype
{
    namespace internal
    {
        // r = stringutils::replace_tokens("__cdecl ", "", r);
        // r = stringutils::replace_tokens("struct ", "", r);
        // r = stringutils::replace_tokens("class ", "", r);

        inline std::string format_whitespace(const std::string & str_type)
        {
            std::string r = str_type;
            r = stringutils::replace_tokens("\t", " ", r);
            r = stringutils::replace_tokens("\r\n", " ", r);
            r = stringutils::replace_tokens("\n", " ", r);
            r = stringutils::replace_tokens("  ", " ", r);
            r = stringutils::replace_tokens("  ", " ", r);
            r = stringutils::replace_tokens("  ", " ", r);
            r = stringutils::insert_spaces_after(',', r);
            r = stringutils::insert_spaces_before_after('&', r);
            r = stringutils::insert_spaces_before_after('*', r);
            r = stringutils::remove_spaces_before_after(')', r);
            r = stringutils::remove_spaces_before_after('(', r);
            r = stringutils::remove_spaces_before('>', r);
            r = stringutils::remove_spaces_after('<', r);
            r = stringutils::remove_spaces_after('[', r);
            r = stringutils::remove_spaces_before(']', r);
            r = stringutils::insert_spaces_before('[', r);
            r = stringutils::insert_spaces_after(']', r);
            r = stringutils::replace_tokens("*&", "* &", r);
            r = stringutils::replace_tokens("&*", "& *", r);
            r = stringutils::replace_tokens("& &", "&&", r);
            r = stringutils::replace_tokens("[ ]", "[]", r);
            r = stringutils::replace_tokens(" ,", ",", r);
            r = stringutils::replace_tokens("* *", "**", r);
            r = cleantype_fp::trim(' ', r);
            return r;
        }

    }  // namespace internal

}  // namespace cleantype
