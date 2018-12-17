#ifndef BOOST_HANA_EXPERIMENTAL_DETAIL_TYPE_NAME_FORMAT_HPP
#define BOOST_HANA_EXPERIMENTAL_DETAIL_TYPE_NAME_FORMAT_HPP
#include <string>

namespace boost {
namespace hana {
namespace experimental {
namespace type_name_format {
namespace detail {
    inline std::string trim(const char &trim_what, const std::string &xs)
    {
        bool stop = false;
        std::string out;
        for (const auto &v : xs)
        {
            if (stop)
                out.push_back(v);
            else if (v != trim_what)
            {
                out.push_back(v);
                stop = true;
            }
        }
        while ((!out.empty()) && (out.back() == trim_what))
            out.pop_back();
        return out;
    }

    inline std::string remove_spaces_before(const char token, const std::string &str)
    {
        std::string result;
        bool space_before = false;
        for (auto c : str)
        {
            if ((c == token) && space_before)
            {
                result.pop_back();
            }
            result = result + c;

            if (c == ' ')
                space_before = true;
            else
                space_before = false;
        }
        return result;
    }

    inline std::string insert_spaces_before(const char token, const std::string &str)
    {
        std::string result;
        bool space_or_same_token_before = true;
        for (auto c : str)
        {
            if ((c == token) && !(space_or_same_token_before))
                result = result + " ";
            result = result + c;
            if ((c == ' ') || (c == token))
                space_or_same_token_before = true;
            else
                space_or_same_token_before = false;
        }
        return result;
    }

    inline std::string insert_spaces_after(const char token, const std::string &str)
    {
        std::string result;
        bool token_before = false;
        for (auto c : str)
        {
            if (token_before && (c != ' '))
                result = result + ' ';
            result = result + c;
            token_before = (c == token);
        }
        return result;
    }

    inline std::string insert_spaces_before_after(const char token, const std::string &str)
    {
        std::string result = insert_spaces_before(token, str);
        result = insert_spaces_after(token, result);
        return result;
    }

    inline std::string replace_tokens(const std::string& from, const std::string& to, const std::string& xs) {
        if(from.empty())
            return xs;
        std::string out = xs;
        std::size_t start_pos = 0;
        while((start_pos = out.find(from, start_pos)) != std::string::npos)
        {
            out.replace(start_pos, from.length(), to);
            start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
        }
        return out;
    }

} // namespace detail


inline std::string format_type(const std::string &str_type)
{
    std::string r = str_type;
    r = detail::insert_spaces_after(',', r);
    r = detail::insert_spaces_before_after('&', r);
    r = detail::insert_spaces_before_after('*', r);
    r = detail::insert_spaces_before_after('[', r);
    r = detail::replace_tokens("*&", "* &", r);
    r = detail::replace_tokens("&*", "& *", r);
    r = detail::replace_tokens("& &", "&&", r);
    r = detail::replace_tokens(" ,", ",", r);
    return str_type;
}

} // namespace type_name_format
} // namespace experimental
} // namespace hana
} // namespace boost

#endif // #define BOOST_HANA_EXPERIMENTAL_DETAIL_TYPE_NAME_FORMAT_HPP
