#include <sstream>
#include <boost/optional.hpp>
#include <cleantype/details/fp_polyfill/fp_polyfill.hpp>
#include <cleantype/details/fp_polyfill/fp_additions.hpp>
#include <cleantype/cleantype.hpp>
#include <cleantype/cleantype_compile_time.hpp>
#include <cleantype/details/stringutils.hpp>

#define LOG_VALUE(var) std::cout << #var << " ==>" << var << "<==\n";

using Strings = std::vector<std::string>;
using String = std::string;
using LineNumbers = std::vector<std::size_t>;
using NumberedLine = std::pair<std::size_t, String>;
using NumberedLines = std::vector<NumberedLine>;


struct CompilerOutputParseConfig
{
    String compiler_error_extract_before_type;
    std::size_t nb_skip_lines_after_extract;
    String start_marker;
    String end_marker;
    String compiler_error_extract_at_call_site;
};

#if defined(__clang__)
CompilerOutputParseConfig MakeCompilerOutputParseConfig()
{
    CompilerOutputParseConfig r;
    r.compiler_error_extract_before_type = _CLEANTYPE_COMPILETIME_MARKER;
    r.nb_skip_lines_after_extract = 0;
    r.start_marker = "boost::hana::string<'";
    r.end_marker = ">' ";
    r.compiler_error_extract_at_call_site = ": note: in instantiation of function template specialization 'cleantype::ERROR_full";
    return r;
}
#elif defined(_MSC_VER)
CompilerOutputParseConfig MakeCompilerOutputParseConfig()
{
    CompilerOutputParseConfig r;
    r.compiler_error_extract_before_type = _CLEANTYPE_COMPILETIME_MARKER;
    r.nb_skip_lines_after_extract = 3;
    r.start_marker = "boost::hana::string<";
    r.end_marker = "> &";
    r.compiler_error_extract_at_call_site = ": note: see reference to function template instantiation ";
    return r;
}
#else
#error("Unsupported compiler")
#endif


auto config = MakeCompilerOutputParseConfig();

String parse_compiler_type(const String & s)
{
#if defined(__clang__)
    // example input: 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'm', 'a', 'p', '<', 'i', 'n', 't', ',', ' ', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'v', 'e', 'c', 't', 'o', 'r', '<', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'b', 'a', 's', 'i', 'c', '_', 's', 't', 'r', 'i', 'n', 'g', '<', 'c', 'h', 'a', 'r', '>', ',', ' ', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'a', 'l', 'l', 'o', 'c', 'a', 't', 'o', 'r', '<', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'b', 'a', 's', 'i', 'c', '_', 's', 't', 'r', 'i', 'n', 'g', '<', 'c', 'h', 'a', 'r', '>', ' ', '>', ' ', '>', ',', ' ', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'l', 'e', 's', 's', '<', 'i', 'n', 't', '>', ',', ' ', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'a', 'l', 'l', 'o', 'c', 'a', 't', 'o', 'r', '<', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'p', 'a', 'i', 'r', '<', 'c', 'o', 'n', 's', 't', ' ', 'i', 'n', 't', ',', ' ', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'v', 'e', 'c', 't', 'o', 'r', '<', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'b', 'a', 's', 'i', 'c', '_', 's', 't', 'r', 'i', 'n', 'g', '<', 'c', 'h', 'a', 'r', '>', ',', ' ', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'a', 'l', 'l', 'o', 'c', 'a', 't', 'o', 'r', '<', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'b', 'a', 's', 'i', 'c', '_', 's', 't', 'r', 'i', 'n', 'g', '<', 'c', 'h', 'a', 'r', '>', ' ', '>', ' ', '>', ' ', '>', ' ', '>', ' ', '>'
    String parsed = fp::replace_tokens("', '", "", s);
    return parsed;
#elif defined(_MSC_VER)
    // example input: 99,108,97,115,115,32,115,116,100,58,58,98,97,115,105,99,95,115,116,114,105,110,103,60,99,104,97,114,44,115,116,114,117,99,116,32,115,116,100,58,58,99,104,97,114,95,116,114,97,105,116,115,60,99,104,97,114,62,44,99,108,97,115,115,32,115,116,100,58,58,97,108,108,111,99,97,116,111,114,60,99,104,97,114,62,32,62
    Strings numbers = cleantype::internal::_split_string(s, ',');
    std::vector<unsigned char> chars;
    for (const auto &str_number : numbers)
        chars.push_back(static_cast<unsigned char>(atoi(str_number.c_str())));
    std::stringstream ss;
    for (auto c : chars)
        ss << c;
    return ss.str();
#endif
}


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
    auto line_extract = extract_content_between_markers(config.start_marker, config.end_marker, compiler_line);
    if ( ! line_extract.is_initialized()) {
        std::cerr << "extract_type_from_compiler_line : failure !\n";
        throw(std::runtime_error("extract_type_from_compiler_line : failure !"));
    }

    auto type_full = parse_compiler_type(line_extract.get());

    return type_full;
}


std::string extract_call_site_after_type(
    const Strings & compiler_lines,
    std::size_t line_idx
    )
{
    while(line_idx < compiler_lines.size() )
    {
        String line = compiler_lines[line_idx];
        auto idx = line.find(config.compiler_error_extract_at_call_site);
        if ( idx != std::string::npos )
        {
            String call_site = line.substr(0, idx);
            return call_site;
        }
        line_idx++;
    }
    return "";
}


bool does_compiler_line_match_error_extract_before_type(const NumberedLine & compiler_line)
{
    return compiler_line.second.find(config.compiler_error_extract_before_type) != String::npos;
}


String extract_types_from_compiler_output(const String & compiler_output, bool flag_clean)
{
    Strings compiler_lines = cleantype::internal::_split_string(compiler_output, '\n');
    NumberedLines numbered_compiler_output = fp::zip( fp::numbers(compiler_lines.size()), compiler_lines);

    NumberedLines lines_before_types = fp::keep_if(does_compiler_line_match_error_extract_before_type, numbered_compiler_output);
    LineNumbers lines_numbers_before_types = fp::unzip(lines_before_types).first;
    LineNumbers lines_numbers_at_types = fp::transform(
        [&](const std::size_t & line_number) { 
            return line_number + config.nb_skip_lines_after_extract;
        }, 
        lines_numbers_before_types);
    Strings lines_with_types = fp::fp_add::take_at_idxs(compiler_lines, lines_numbers_at_types);
    Strings types_full = fp::transform(extract_type_from_compiler_line, lines_with_types);
    Strings types_clean;
    if (flag_clean)
        types_clean = fp::transform(cleantype::internal::impl_clean_several_types, types_full);
    else
        types_clean = types_full;
    
    Strings call_sites;
    {
        auto extract_call_site = [&](const std::size_t & line_number) {
            return extract_call_site_after_type(compiler_lines, line_number);
        };
        call_sites = fp::transform<decltype(extract_call_site), std::size_t, String> (extract_call_site, lines_numbers_at_types);
    }

    std::size_t nb_types = lines_numbers_at_types.size();
    Strings output;
    for (auto i : fp::numbers(nb_types))
    {
        String s = types_clean[i] + "\n\tat: " + call_sites[i] + " (Compiler line #" + fp::show(lines_numbers_at_types[i]) + ")";
        output.push_back(s);
    }

    return fp::join("\n\n", output);
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
