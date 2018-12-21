#include <boost/optional.hpp>
#include <cleantype/details/fp_polyfill/fp_polyfill.hpp>
#include <cleantype/cleantype.hpp>
#include <cleantype/cleantype_compile_time.hpp>

#define LOG_VALUE(var) std::cout << #var << " ==>" << var << "<==\n";

using Strings = std::vector<std::string>;
using String = std::string;

boost::optional<String> extract_content_between_markers(
    const std::string & start_marker,
    const std::string & end_marker,
    const std::string & s
)
{
    std::size_t idx_start;
    {
        std::size_t idx = s.find(start_marker);
        if (idx == String::npos)
            return boost::none;
        idx_start = idx + start_marker.size();
    }
    std::size_t idx_end;
    {
        idx_end = s.find(end_marker, idx_start + 1);
        if (idx_end == String::npos)
            return boost::none;
    }

    String r = s.substr(idx_start, idx_end - idx_start);
    return r;
}

String extract_type_from_compiler_line( // can throw runtime_error !
    const String & compiler_line
    )
{
    const String start_marker = "boost::hana::string<'";
    const String end_marker = "'>' ";
    auto line_extract = extract_content_between_markers(start_marker, end_marker, compiler_line);
    if ( ! line_extract.is_initialized()) {
        std::cerr << "extract_type_from_compiler_line : failure !\n";
        throw(std::runtime_error("extract_type_from_compiler_line : failure !"));
    }

    auto type_full = fp::replace_tokens("', '", "", line_extract.get());

    return type_full;
}

bool does_compiler_line_match_marker(const String & compiler_line)
{
    return compiler_line.find(_CLEANTYPE_COMPILETIME_MARKER) != String::npos;
}

String extract_types_from_compiler_output(const String & compiler_output, bool flag_clean)
{
    Strings lines = cleantype::internal::_split_string(compiler_output, '\n');
    Strings lines_match = fp::keep_if(does_compiler_line_match_marker, lines);
    Strings extracted_types = fp::transform(extract_type_from_compiler_line, lines_match);

    if (flag_clean)
        extracted_types = fp::transform(cleantype::internal::impl_clean_several_types, extracted_types);

    return fp::join("\n", extracted_types);
}

int main(int argc, char ** argv)
{
    bool flag_clean = false;
    if (argc >= 2) {
        String arg1(argv[1]);
        if ( ( arg1 == "--clean" ) || (arg1 == "-c"))
            flag_clean = true;
    }
    auto curried = [&](const String &s) -> String {
        auto r = extract_types_from_compiler_output(s, flag_clean);
        if (! r.empty())
            r = r + "\n";
        else
            return "Did not find any type in the compiler output!\n";
        return r;
    };
    auto prog = fp::interact(curried);
    prog();
    return 0;
}
