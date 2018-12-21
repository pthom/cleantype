#include <cleantype/details/fp_polyfill/fp_polyfill.hpp>
#include <cleantype/cleantype.hpp>
#include <cleantype/cleantype_compile_time.hpp>

#define LOG_VALUE(var) std::cout << #var << " ==>" << var << "<==\n";

using Strings = std::vector<std::string>;

std::string extract_type_from_compiler_line(
    const std::string & compiler_line
    )
{
    const std::string start_marker = "boost::hana::string<'";
    const std::string end_marker = "'>' ";

    std::size_t idx_start;
    {
        std::size_t idx = compiler_line.find(start_marker);
        if (idx == std::string::npos)
            return "";
        idx_start = idx + start_marker.size();
    }
    std::size_t idx_end;
    {
        idx_end = compiler_line.find(end_marker, idx_start + 1);
        if (idx_end == std::string::npos)
            return "";
    }

    auto line_extract = compiler_line.substr(idx_start, idx_end - idx_start);
    auto type_full = fp::replace_tokens("', '", "", line_extract);

    return type_full;
}

bool does_compiler_line_match_marker(const std::string & compiler_line)
{
    return compiler_line.find(_CLEANTYPE_COMPILETIME_MARKER) != std::string::npos;
}

std::string extract_types_from_compiler_output(const std::string & compiler_output, bool flag_clean)
{
    Strings lines = cleantype::internal::_split_string(compiler_output, '\n');
    Strings lines_match = fp::keep_if(does_compiler_line_match_marker, lines);
    Strings extracted_types = fp::transform(extract_type_from_compiler_line, lines_match);

    if (flag_clean)
        extracted_types = fp::transform(cleantype::internal::impl_clean_several_types, extracted_types);

    return fp::join("\n", extracted_types) + "\n";
}

int main()
{
    bool flag_clean = true;
    auto curried = [&](const std::string &s) {
        return extract_types_from_compiler_output(s, flag_clean);
    };
    auto prog = fp::interact(curried);
    prog();
    return 0;
}
