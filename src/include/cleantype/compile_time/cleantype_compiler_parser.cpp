#include <boost/optional.hpp>
#include <cleantype/details/fp_polyfill/fp_polyfill.hpp>
#include <cleantype/cleantype.hpp>
#include <cleantype/cleantype_compile_time.hpp>
#include <cleantype/details/stringutils.hpp>

#define LOG_VALUE(var) std::cout << #var << " ==>" << var << "<==\n";

using Strings = std::vector<std::string>;
using String = std::string;
using NumberedLine = std::pair<std::size_t, String>;
using NumberedLines = std::vector<NumberedLine>;

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

bool does_compiler_line_match_marker(const NumberedLine & compiler_line)
{
    return compiler_line.second.find(_CLEANTYPE_COMPILETIME_MARKER) != String::npos;
}

std::string extract_call_site_after_type(
    const NumberedLines & compiler_output,
    std::size_t line_idx
    )
{
    /*
    // Example of an error output (clang)
../src/include/cleantype/cleantype_compile_time.hpp:38:8: error: type 'boost::hana::string<'s', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'm', 'a', 'p', '<', 'i', 'n', 't', ',', ' ', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'v', 'e', 'c', 't', 'o', 'r', '<', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'b', 'a', 's', 'i', 'c', '_', 's', 't', 'r', 'i', 'n', 'g', '<', 'c', 'h', 'a', 'r', '>', ',', ' ', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'a', 'l', 'l', 'o', 'c', 'a', 't', 'o', 'r', '<', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'b', 'a', 's', 'i', 'c', '_', 's', 't', 'r', 'i', 'n', 'g', '<', 'c', 'h', 'a', 'r', '>', ' ', '>', ' ', '>', ',', ' ', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'l', 'e', 's', 's', '<', 'i', 'n', 't', '>', ',', ' ', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'a', 'l', 'l', 'o', 'c', 'a', 't', 'o', 'r', '<', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'p', 'a', 'i', 'r', '<', 'c', 'o', 'n', 's', 't', ' ', 'i', 'n', 't', ',', ' ', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'v', 'e', 'c', 't', 'o', 'r', '<', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'b', 'a', 's', 'i', 'c', '_', 's', 't', 'r', 'i', 'n', 'g', '<', 'c', 'h', 'a', 'r', '>', ',', ' ', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'a', 'l', 'l', 'o', 'c', 'a', 't', 'o', 'r', '<', 's', 't', 'd', ':', ':', '_', '_', '1', ':', ':', 'b', 'a', 's', 'i', 'c', '_', 's', 't', 'r', 'i', 'n', 'g', '<', 'c', 'h', 'a', 'r', '>', ' ', '>', ' ', '>', ' ', '>', ' ', '>', ' ', '>'>' does not provide a call operator
       intentional_error(); // your type can be deciphered via : make 2>&1 | cleantype_compiler_parser [-c | --clean]
       ^~~~~~~~~~~~~~~~~
../src/include/cleantype/compile_time/example/cleantype_compile_time_example.cpp:8:16: note: in instantiation of function template specialization 'cleantype::ERROR_full<std::__1::map<int, std::__1::vector<std::__1::basic_string<char>, std::__1::allocator<std::__1::basic_string<char> > >, std::__1::less<int>, std::__1::allocator<std::__1::pair<const int, std::__1::vector<std::__1::basic_string<char>, std::__1::allocator<std::__1::basic_string<char> > > > > > >' requested here
    cleantype::ERROR_full<std::map<int, std::vector<std::string>>>();
               ^
    */
    while(line_idx < compiler_output.size() )
    {
        String line = compiler_output[line_idx].second;
        if ( cleantype::stringutils::ends_with(line, "requested here") )
        {
            String clang_error_start = ": note: in instantiation of ";
            auto idx_end_extract = line.find(clang_error_start);
            if (idx_end_extract != std::string::npos)
            {
                String call_site = line.substr(0, idx_end_extract);
                if (line_idx < compiler_output.size() - 2)
                    call_site = call_site + "\n==>" + compiler_output[line_idx + 1].second;
                return call_site;
            }
        }
        line_idx++;
    }
    return "";
}

String extract_types_from_compiler_output(const String & compiler_output, bool flag_clean)
{
    Strings lines = cleantype::internal::_split_string(compiler_output, '\n');
    NumberedLines numbered_compiler_output = fp::zip( fp::numbers(lines.size()), lines );
    NumberedLines lines_with_types = fp::keep_if(does_compiler_line_match_marker, numbered_compiler_output);

    NumberedLines extracted_types = fp::transform(
        [flag_clean](const NumberedLine & nl) -> NumberedLine {
            String type_def = extract_type_from_compiler_line(nl.second);
            if (flag_clean)
                type_def = cleantype::internal::impl_clean_several_types(type_def);
            return { nl.first, type_def };
        },
        lines_with_types);

    NumberedLines extracted_types_with_call_site = fp::transform(
        [&](const NumberedLine & nl) -> NumberedLine {
            String type_with_call_site = nl.second + "\n\tat "
            + extract_call_site_after_type(numbered_compiler_output, nl.first);
            return { nl.first, type_with_call_site };
        },
        extracted_types);

    auto show_type_with_call_site = [](const NumberedLine & nl) -> String {
        return "compiler output line #" + fp::show(nl.first + 1) + ":\n"
                + nl.second;
    };

    Strings show_extracted_types_with_call_site =
        fp::transform<decltype(show_type_with_call_site), NumberedLine, String>(
            show_type_with_call_site,
            extracted_types_with_call_site);

    return fp::join("\n\n", show_extracted_types_with_call_site);
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
