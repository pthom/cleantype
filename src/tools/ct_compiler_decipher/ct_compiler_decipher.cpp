#include <cleantype/cleantype.hpp>
#include <cleantype/details/cleantype_fp/fp_interact.hpp>
#include <sstream>

struct template_count
{
    int nb_open_close = 0;
    int nb_remaining_open = 0;
};

template_count count_templates(const std::string & compiler_line)
{
    template_count r;
    for (auto c : compiler_line)
    {
        if (c == '<')
        {
            r.nb_open_close++;
            r.nb_remaining_open++;
        }
        if (c == '>')
        {
            r.nb_open_close++;
            r.nb_remaining_open--;
        }
    }
    return r;
}

bool need_decipher_line(const std::string & compiler_line)
{
    auto r = count_templates(compiler_line);

    if (r.nb_remaining_open != 0)
        return false;
    return (r.nb_open_close >= 2);
}

std::string msvc_remove_false_open_template(const std::string & compiler_line)
{
#ifdef _MSC_VER
    // msvc compile lines start with a false opening template (">") => we skip it
    // example :
    // 1>F:\dvp\OpenSource\type_name\src\include\cleantype/details/cleantype_fp/fp_base.hpp(38): ...

    auto idx = compiler_line.find(">");
    if ((idx != std::string::npos) && (idx < 5))
    {
        std::string cut = compiler_line.substr(idx + 1, compiler_line.size() - idx);
        return cut;
    }

    return compiler_line;
#else
    return compiler_line;
#endif
}

std::string decipher_line(const std::string & compiler_line)
{
    std::string compiler_line_cut = msvc_remove_false_open_template(compiler_line);
    bool needs_decipher = need_decipher_line(compiler_line_cut);
    if (needs_decipher)
        // return std::string("PROCESSED\n") + cleantype::clean_typestring(compiler_line_cut) +
        // "\n\n";
        return cleantype::clean_typestring(compiler_line_cut);
    else
        // return std::string("BARE\n") + compiler_line + "\n\n";
        return compiler_line;
}

int main()
{
    auto prog = cleantype_fp_interact::interact_by_line(decipher_line);

    // Debug version
    //{
    //    std::string error_log = R"(
    //    )";
    //    std::istringstream is(error_log);
    //    auto prog = cleantype_fp::interact_by_line(decipher_line, is, std::cout);
    //}

    prog();
}
