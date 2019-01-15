#include <cleantype/details/fp_polyfill/fp_additions.hpp>
#include <cleantype/cleantype.hpp>
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
    // 1>F:\dvp\OpenSource\type_name\src\include\cleantype/details/fp_polyfill/fp_polyfill.hpp(38): ...

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
        //return std::string("PROCESSED\n") + cleantype::clean_typestring(compiler_line_cut) + "\n\n";
        return cleantype::clean_typestring(compiler_line_cut);
    else
        //return std::string("BARE\n") + compiler_line + "\n\n";
        return compiler_line;
}

int main()
{
    auto prog = fp::fp_add::interact_by_line(decipher_line);

    // Debug version
    //{
    //    std::string error_log = R"(
    //    )";
    //    std::istringstream is(error_log);
    //    auto prog = fp::interact_by_line(decipher_line, is, std::cout);
    //}

    prog();
}

/*

Error log with decipher / CLANG
********************************

ninja | ./cleantype_decipherer
[1/2] Building CXX object src/include/cleantype/compile_time/CMakeFiles/cleantype_decipher_example.dir/example/cleantype_decipher_example.cpp.o
FAILED: src/include/cleantype/compile_time/CMakeFiles/cleantype_decipher_example.dir/example/cleantype_decipher_example.cpp.o
ccache /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  -DCLEANTYPE_COMPILETIME -I../third_party/FunctionalPlus/include -I../src/include -I/Users/pascal/.conan/data/boost/1.68.0/conan/stable/package/411d816e9b8d6cb0bb1ef6e5be8b2a94b8b17c04/include -std=c++14 -MD -MT src/include/cleantype/compile_time/CMakeFiles/cleantype_decipher_example.dir/example/cleantype_decipher_example.cpp.o -MF src/include/cleantype/compile_time/CMakeFiles/cleantype_decipher_example.dir/example/cleantype_decipher_example.cpp.o.d -o src/include/cleantype/compile_time/CMakeFiles/cleantype_decipher_example.dir/example/cleantype_decipher_example.cpp.o -c ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: error: invalid operands to binary expression ('const std::map<std::string, int> ' and 'int')
return v + 1;
~ ^ ~
../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:38:23: note: in instantiation of function template specialization '(anonymous class)::operator()<std::map<std::string, int>> ' requested here
out.push_back(f(x));
^
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:35:19: note: in instantiation of function template specialization 'fp::transform<(lambda at ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:25:28), std::map<std::string, int>, std::map<std::string, int>> ' requested here
auto v3 = fp::transform(my_spurious_lambda3, v2);
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/iterator:761:1: note: candidate template ignored: could not match 'reverse_iterator<type-parameter-0-0> ' against 'int'
operator+(typename reverse_iterator<_Iter> ::difference_type __n, const reverse_iterator<_Iter> & __x)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/iterator:1202:1: note: candidate template ignored: could not match 'move_iterator<type-parameter-0-0> ' against 'int'
operator+(typename move_iterator<_Iter> ::difference_type __n, const move_iterator<_Iter> & __x)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/iterator:1473:5: note: candidate template ignored: could not match '__wrap_iter<type-parameter-0-0> ' against 'int'
operator+(typename __wrap_iter<_Iter1> ::difference_type, __wrap_iter<_Iter1> ) _NOEXCEPT_DEBUG;
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3765:1: note: candidate template ignored: could not match 'basic_string' against 'map'
operator+(const std::string & __lhs,
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3778:1: note: candidate template ignored: could not match 'const _CharT *' against 'std::map<std::string, int> '
operator+(const _CharT* __lhs, const std::string & __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3790:1: note: candidate template ignored: could not match 'basic_string<type-parameter-0-0, type-parameter-0-1, type-parameter-0-2> ' against 'int'
operator+(_CharT __lhs, const std::string & __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3801:1: note: candidate template ignored: could not match 'basic_string' against 'map'
operator+(const std::string & __lhs, const _CharT* __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3813:1: note: candidate template ignored: could not match 'basic_string' against 'map'
operator+(const std::string & __lhs, _CharT __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3827:1: note: candidate template ignored: could not match 'basic_string' against 'map'
operator+(std::string && __lhs, const std::string & __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3835:1: note: candidate template ignored: could not match 'basic_string' against 'map'
operator+(const std::string & __lhs, std::string && __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3843:1: note: candidate template ignored: could not match 'basic_string' against 'map'
operator+(std::string && __lhs, std::string && __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3851:1: note: candidate template ignored: could not match 'const _CharT *' against 'std::map<std::string, int> '
operator+(const _CharT* __lhs, std::string && __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3859:1: note: candidate template ignored: could not match 'basic_string<type-parameter-0-0, type-parameter-0-1, type-parameter-0-2> ' against 'int'
operator+(_CharT __lhs, std::string && __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3868:1: note: candidate template ignored: could not match 'basic_string' against 'map'
operator+(std::string && __lhs, const _CharT* __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3876:1: note: candidate template ignored: could not match 'basic_string' against 'map'
operator+(std::string && __lhs, _CharT __rhs)
^
1 error generated.
ninja: build stopped: subcommand failed.



Original error log
******************

✗ ninja
[1/2] Building CXX object src/include/cleantype/compile_time/CMakeFiles/cleantype_decipher_example.dir/example/cleantype_decipher_example.cpp.o
FAILED: src/include/cleantype/compile_time/CMakeFiles/cleantype_decipher_example.dir/example/cleantype_decipher_example.cpp.o
ccache /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  -DCLEANTYPE_COMPILETIME -I../third_party/FunctionalPlus/include -I../src/include -I/Users/pascal/.conan/data/boost/1.68.0/conan/stable/package/411d816e9b8d6cb0bb1ef6e5be8b2a94b8b17c04/include -std=c++14 -MD -MT src/include/cleantype/compile_time/CMakeFiles/cleantype_decipher_example.dir/example/cleantype_decipher_example.cpp.o -MF src/include/cleantype/compile_time/CMakeFiles/cleantype_decipher_example.dir/example/cleantype_decipher_example.cpp.o.d -o src/include/cleantype/compile_time/CMakeFiles/cleantype_decipher_example.dir/example/cleantype_decipher_example.cpp.o -c ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: error: invalid operands to binary expression ('const std::__1::map<std::__1::basic_string<char>, int, std::__1::less<std::__1::basic_string<char> >, std::__1::allocator<std::__1::pair<const std::__1::basic_string<char>, int> > >' and 'int')
    return v + 1;
           ~ ^ ~
../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:38:23: note: in instantiation of function template specialization '(anonymous class)::operator()<std::__1::map<std::__1::basic_string<char>, int, std::__1::less<std::__1::basic_string<char> >, std::__1::allocator<std::__1::pair<const std::__1::basic_string<char>, int> > > >' requested here
        out.push_back(f(x));
                      ^
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:35:19: note: in instantiation of function template specialization 'fp::transform<(lambda at ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:25:28), std::__1::map<std::__1::basic_string<char>, int, std::__1::less<std::__1::basic_string<char> >, std::__1::allocator<std::__1::pair<const std::__1::basic_string<char>, int> > >, std::__1::map<std::__1::basic_string<char>, int, std::__1::less<std::__1::basic_string<char> >, std::__1::allocator<std::__1::pair<const std::__1::basic_string<char>, int> > > >' requested here
    auto v3 = fp::transform(my_spurious_lambda3, v2);
                  ^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/iterator:761:1: note: candidate template ignored: could not match 'reverse_iterator<type-parameter-0-0>' against 'int'
operator+(typename reverse_iterator<_Iter>::difference_type __n, const reverse_iterator<_Iter>& __x)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/iterator:1202:1: note: candidate template ignored: could not match 'move_iterator<type-parameter-0-0>' against 'int'
operator+(typename move_iterator<_Iter>::difference_type __n, const move_iterator<_Iter>& __x)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/iterator:1473:5: note: candidate template ignored: could not match '__wrap_iter<type-parameter-0-0>' against 'int'
    operator+(typename __wrap_iter<_Iter1>::difference_type, __wrap_iter<_Iter1>) _NOEXCEPT_DEBUG;
    ^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3765:1: note: candidate template ignored: could not match 'basic_string' against 'map'
operator+(const basic_string<_CharT, _Traits, _Allocator>& __lhs,
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3778:1: note: candidate template ignored: could not match 'const _CharT *' against 'std::__1::map<std::__1::basic_string<char>, int, std::__1::less<std::__1::basic_string<char> >, std::__1::allocator<std::__1::pair<const std::__1::basic_string<char>, int> > >'
operator+(const _CharT* __lhs , const basic_string<_CharT,_Traits,_Allocator>& __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3790:1: note: candidate template ignored: could not match 'basic_string<type-parameter-0-0, type-parameter-0-1, type-parameter-0-2>' against 'int'
operator+(_CharT __lhs, const basic_string<_CharT,_Traits,_Allocator>& __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3801:1: note: candidate template ignored: could not match 'basic_string' against 'map'
operator+(const basic_string<_CharT, _Traits, _Allocator>& __lhs, const _CharT* __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3813:1: note: candidate template ignored: could not match 'basic_string' against 'map'
operator+(const basic_string<_CharT, _Traits, _Allocator>& __lhs, _CharT __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3827:1: note: candidate template ignored: could not match 'basic_string' against 'map'
operator+(basic_string<_CharT, _Traits, _Allocator>&& __lhs, const basic_string<_CharT, _Traits, _Allocator>& __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3835:1: note: candidate template ignored: could not match 'basic_string' against 'map'
operator+(const basic_string<_CharT, _Traits, _Allocator>& __lhs, basic_string<_CharT, _Traits, _Allocator>&& __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3843:1: note: candidate template ignored: could not match 'basic_string' against 'map'
operator+(basic_string<_CharT, _Traits, _Allocator>&& __lhs, basic_string<_CharT, _Traits, _Allocator>&& __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3851:1: note: candidate template ignored: could not match 'const _CharT *' against 'std::__1::map<std::__1::basic_string<char>, int, std::__1::less<std::__1::basic_string<char> >, std::__1::allocator<std::__1::pair<const std::__1::basic_string<char>, int> > >'
operator+(const _CharT* __lhs , basic_string<_CharT,_Traits,_Allocator>&& __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3859:1: note: candidate template ignored: could not match 'basic_string<type-parameter-0-0, type-parameter-0-1, type-parameter-0-2>' against 'int'
operator+(_CharT __lhs, basic_string<_CharT,_Traits,_Allocator>&& __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3868:1: note: candidate template ignored: could not match 'basic_string' against 'map'
operator+(basic_string<_CharT, _Traits, _Allocator>&& __lhs, const _CharT* __rhs)
^
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/string:3876:1: note: candidate template ignored: could not match 'basic_string' against 'map'
operator+(basic_string<_CharT, _Traits, _Allocator>&& __lhs, _CharT __rhs)
^
1 error generated.
ninja: build stopped: subcommand failed.
*/





/*

Error log with decipher / MSVC
********************************


1>------ Build started: Project: cleantype_decipher_example, Configuration: Debug x64 ------
2>------ Build started: Project: cleantype_decipherer, Configuration: Debug x64 ------
3>------ Skipped Build: Project: RUN_TESTS, Configuration: Debug x64 ------
3>Project not selected to build for this solution configuration
1>cleantype_decipher_example.cpp
F:\dvp\OpenSource\type_name\src\include\cleantype\compile_time\example\cleantype_decipher_example.cpp(26): error C2676: binary '+': 'const std::map<std::string, int> ' does not define this operator or a conversion to a type acceptable to the predefined operator
1>        with
1>        [
1>            _Kty=fplus::FunctionName,
1>            _Ty=int
1>        ]
F:\dvp\OpenSource\type_name\src\include\cleantype/details/fp_polyfill/fp_polyfill.hpp(38): note: see reference to function template instantiation 'auto<lambda_2274920e60722aee18d17f25952d5f34, std::map<std::string, int>, _Kty, _Ty> ::operator ()(const std::map &) const' being compiled
1>        with
1>        [
1>            _Kty=fplus::FunctionName,
1>            _Ty=int
1>        ]
F:\dvp\OpenSource\type_name\src\include\cleantype\compile_time\example\cleantype_decipher_example.cpp(35): note: see reference to function template instantiation 'std::vector<std::map<std::string, int>, <lambda_2274920e60722aee18d17f25952d5f34>, std::map<_Kty, _Ty>, T> fp::transform(F, const std::vector<std::map<_Kty, _Ty>> &)' being compiled
1>        with
1>        [
1>            _Kty=fplus::FunctionName,
1>            _Ty=int,
T=std::map<std::string, int>,
F=<lambda_2274920e60722aee18d17f25952d5f34>
1>        ]
C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Tools\MSVC\14.16.27023\include\chrono(201): note: see reference to template instantiation 'std::chrono::duration<__int64, std::nano> ' being compiled
C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Tools\MSVC\14.16.27023\include\chrono(799): note: see reference to template instantiation 'std::chrono::time_point<std::chrono::steady_clock, std::chrono::steady_clock::duration> ' being compiled
F:\dvp\OpenSource\type_name\src\include\cleantype/details/fp_polyfill/fp_polyfill.hpp(38): error C2664: 'void std::vector<std::map<std::string, int>, _Kty, _Ty> ::push_back(std::map &&)': cannot convert argument 1 from 'void' to 'const _Ty &'
1>        with
1>        [
1>            _Kty=fplus::FunctionName,
1>            _Ty=int
1>        ]
1>        and
1>        [
_Ty=std::map<std::string, int>
1>        ]
1>F:\dvp\OpenSource\type_name\src\include\cleantype/details/fp_polyfill/fp_polyfill.hpp(38): note: Expressions of type void cannot be converted to other types
1>Done building project "cleantype_decipher_example.vcxproj" -- FAILED.
2>cleantype_decipherer.cpp
2>cleantype_decipherer.vcxproj -> F:\dvp\OpenSource\type_name\build\bin\cleantype_decipherer.exe
4>------ Skipped Build: Project: ALL_BUILD, Configuration: Debug x64 ------
4>Project not selected to build for this solution configuration
========== Build: 1 succeeded, 1 failed, 5 up-to-date, 2 skipped ==========


Error log without decipher / MSVC
********************************

1>------ Build started: Project: cleantype_decipher_example, Configuration: Debug x64 ------
2>------ Build started: Project: cleantype_decipherer, Configuration: Debug x64 ------
3>------ Skipped Build: Project: RUN_TESTS, Configuration: Debug x64 ------
3>Project not selected to build for this solution configuration
1>cleantype_decipher_example.cpp
1>F:\dvp\OpenSource\type_name\src\include\cleantype\compile_time\example\cleantype_decipher_example.cpp(26): error C2676: binary '+': 'const std::map<std::string,int,std::less<_Kty>,std::allocator<std::pair<const _Kty,_Ty>>>' does not define this operator or a conversion to a type acceptable to the predefined operator
1>        with
1>        [
1>            _Kty=fplus::FunctionName,
1>            _Ty=int
1>        ]
1>F:\dvp\OpenSource\type_name\src\include\cleantype/details/fp_polyfill/fp_polyfill.hpp(38): note: see reference to function template instantiation 'auto <lambda_2274920e60722aee18d17f25952d5f34>::operator ()<std::map<std::string,int,std::less<_Kty>,std::allocator<std::pair<const _Kty,_Ty>>>>(const std::map<_Kty,_Ty,std::less<_Kty>,std::allocator<std::pair<const _Kty,_Ty>>> &) const' being compiled
1>        with
1>        [
1>            _Kty=fplus::FunctionName,
1>            _Ty=int
1>        ]
1>F:\dvp\OpenSource\type_name\src\include\cleantype\compile_time\example\cleantype_decipher_example.cpp(35): note: see reference to function template instantiation 'std::vector<std::map<std::string,int,std::less<_Kty>,std::allocator<std::pair<const _Kty,_Ty>>>,std::allocator<std::map<_Kty,_Ty,std::less<_Kty>,std::allocator<std::pair<const _Kty,_Ty>>>>> fp::transform<<lambda_2274920e60722aee18d17f25952d5f34>,std::map<_Kty,_Ty,std::less<_Kty>,std::allocator<std::pair<const _Kty,_Ty>>>,T>(F,const std::vector<std::map<_Kty,_Ty,std::less<_Kty>,std::allocator<std::pair<const _Kty,_Ty>>>,std::allocator<std::map<_Kty,_Ty,std::less<_Kty>,std::allocator<std::pair<const _Kty,_Ty>>>>> &)' being compiled
1>        with
1>        [
1>            _Kty=fplus::FunctionName,
1>            _Ty=int,
1>            T=std::map<std::string,int,std::less<fplus::FunctionName>,std::allocator<std::pair<const std::string,int>>>,
1>            F=<lambda_2274920e60722aee18d17f25952d5f34>
1>        ]
1>C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Tools\MSVC\14.16.27023\include\chrono(201): note: see reference to class template instantiation 'std::chrono::duration<__int64,std::nano>' being compiled
1>C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Tools\MSVC\14.16.27023\include\chrono(799): note: see reference to class template instantiation 'std::chrono::time_point<std::chrono::steady_clock,std::chrono::steady_clock::duration>' being compiled
1>F:\dvp\OpenSource\type_name\src\include\cleantype/details/fp_polyfill/fp_polyfill.hpp(38): error C2664: 'void std::vector<std::map<std::string,int,std::less<_Kty>,std::allocator<std::pair<const _Kty,_Ty>>>,std::allocator<std::map<_Kty,_Ty,std::less<_Kty>,std::allocator<std::pair<const _Kty,_Ty>>>>>::push_back(std::map<_Kty,_Ty,std::less<_Kty>,std::allocator<std::pair<const _Kty,_Ty>>> &&)': cannot convert argument 1 from 'void' to 'const _Ty &'
1>        with
1>        [
1>            _Kty=fplus::FunctionName,
1>            _Ty=int
1>        ]
1>        and
1>        [
1>            _Ty=std::map<std::string,int,std::less<fplus::FunctionName>,std::allocator<std::pair<const std::string,int>>>
1>        ]
1>F:\dvp\OpenSource\type_name\src\include\cleantype/details/fp_polyfill/fp_polyfill.hpp(38): note: Expressions of type void cannot be converted to other types
1>Done building project "cleantype_decipher_example.vcxproj" -- FAILED.
2>cleantype_decipherer.cpp
2>cleantype_decipherer.vcxproj -> F:\dvp\OpenSource\type_name\build\bin\cleantype_decipherer.exe
4>------ Skipped Build: Project: ALL_BUILD, Configuration: Debug x64 ------
4>Project not selected to build for this solution configuration
========== Build: 1 succeeded, 1 failed, 5 up-to-date, 2 skipped ==========


*/



/*
Error log with decipher / GCC
*****************************

> ninja | ./cleantype_decipherer
[1/4] Building CXX object src/include/cleantype/compile_time/CMakeFiles/cleantype_decipher_example.dir/example/cleantype_decipher_example.cpp.o
FAILED: src/include/cleantype/compile_time/CMakeFiles/cleantype_decipher_example.dir/example/cleantype_decipher_example.cpp.o
/usr/bin/c++  -DCLEANTYPE_COMPILETIME -I../third_party/FunctionalPlus/include -I../src/include -I/root/.conan/data/boost/1.68.0/conan/stable/package/d6cb600df9137cb7943d00f26e402be306584995/include -std=c++14 -MD -MT src/include/cleantype/compile_time/CMakeFiles/cleantype_decipher_example.dir/example/cleantype_decipher_example.cpp.o -MF src/include/cleantype/compile_time/CMakeFiles/cleantype_decipher_example.dir/example/cleantype_decipher_example.cpp.o.d -o src/include/cleantype/compile_time/CMakeFiles/cleantype_decipher_example.dir/example/cleantype_decipher_example.cpp.o -c ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp: In instantiation of ‘<lambda(const auto:1316 &), std::string, int> [with auto:1316 = std::map]’:
../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:38:24:   required from ‘std::vector<NewT> fp::transform(F, const std::vector<Y, lambda(const auto:1316 &), std::string, int, std::string, int> &) [with F = ; T = std::map; U = std::map]’
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:35:52:   required from here
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: error: no match for ‘operator+’ (operand types are ‘const std::map<std::string, int> ’ and ‘int’)
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/deque:64:0,
                 from ../third_party/FunctionalPlus/include/fplus/container_traits.hpp:10,
                 from ../third_party/FunctionalPlus/include/fplus/container_common.hpp:10,
                 from ../third_party/FunctionalPlus/include/fplus/fplus.hpp:11,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:11,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/stl_deque.h:374:5: note: candidate: template<_Tp, _Ref, _Ptr, _Tp, _Ref, _Ptr> std::_Deque_iterator std::operator+(std::ptrdiff_t, const std::_Deque_iterator<_Tp, _Ref, _Ptr> &)
operator+(ptrdiff_t __n, const _Deque_iterator<_Tp, _Ref, _Ptr> & __x)
     ^~~~~~~~
/usr/include/c++/7/bits/stl_deque.h:374:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   mismatched types ‘const std::_Deque_iterator<_Tp, _Ref, _Ptr> ’ and ‘int’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/vector:65:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:8,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/stl_bvector.h:387:3: note: candidate: std::_Bit_const_iterator std::operator+(std::ptrdiff_t, const std::_Bit_const_iterator&)
   operator+(ptrdiff_t __n, const _Bit_const_iterator& __x)
   ^~~~~~~~
/usr/include/c++/7/bits/stl_bvector.h:387:3: note:   no known conversion for argument 1 from ‘const std::map<std::string, int> ’ to ‘std::ptrdiff_t {aka long int}’
/usr/include/c++/7/bits/stl_bvector.h:297:3: note: candidate: std::_Bit_iterator std::operator+(std::ptrdiff_t, const std::_Bit_iterator&)
   operator+(ptrdiff_t __n, const _Bit_iterator& __x)
   ^~~~~~~~
/usr/include/c++/7/bits/stl_bvector.h:297:3: note:   no known conversion for argument 1 from ‘const std::map<std::string, int> ’ to ‘std::ptrdiff_t {aka long int}’
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5986:5: note: candidate: template<_CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc> std::basic_string std::operator+(std::basic_string &&, _CharT)
operator+(basic_string<_CharT, _Traits, _Alloc> && __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5986:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   types ‘std::basic_string<_CharT, _Traits, _Alloc, std::string, int> ’ and ‘const std::map’ have incompatible cv-qualifiers
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5980:5: note: candidate: template<_CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc> std::basic_string std::operator+(std::basic_string &&, const _CharT*)
operator+(basic_string<_CharT, _Traits, _Alloc> && __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5980:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   types ‘std::basic_string<_CharT, _Traits, _Alloc, std::string, int> ’ and ‘const std::map’ have incompatible cv-qualifiers
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5974:5: note: candidate: template<_CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc> std::basic_string std::operator+(_CharT, std::basic_string<_CharT, _Traits, _Alloc> &&)
     operator+(_CharT __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5974:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   mismatched types ‘std::basic_string<_CharT, _Traits, _Alloc> ’ and ‘int’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5968:5: note: candidate: template<_CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc> std::basic_string std::operator+(const _CharT*, std::basic_string<_CharT, _Traits, _Alloc> &&)
     operator+(const _CharT* __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5968:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   mismatched types ‘const _CharT*’ and ‘std::map<std::string, int> ’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5956:5: note: candidate: template<_CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc> std::basic_string std::operator+(std::basic_string &&, std::basic_string<_CharT, _Traits, _Alloc> &&)
operator+(basic_string<_CharT, _Traits, _Alloc> && __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5956:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   types ‘std::basic_string<_CharT, _Traits, _Alloc, std::string, int> ’ and ‘const std::map’ have incompatible cv-qualifiers
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5950:5: note: candidate: template<_CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc> std::basic_string std::operator+(const std::basic_string &, std::basic_string<_CharT, _Traits, _Alloc> &&)
operator+(const basic_string<_CharT, _Traits, _Alloc> & __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5950:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   ‘const std::map<std::string, int, _CharT, _Traits, _Alloc> ’ is not derived from ‘const std::basic_string’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5944:5: note: candidate: template<_CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc> std::basic_string std::operator+(std::basic_string &&, const std::basic_string<_CharT, _Traits, _Alloc> &)
operator+(basic_string<_CharT, _Traits, _Alloc> && __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5944:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   types ‘std::basic_string<_CharT, _Traits, _Alloc, std::string, int> ’ and ‘const std::map’ have incompatible cv-qualifiers
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5932:5: note: candidate: template<_CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc> std::basic_string std::operator+(const std::basic_string &, _CharT)
operator+(const basic_string<_CharT, _Traits, _Alloc> & __lhs, _CharT __rhs)
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5932:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   ‘const std::map<std::string, int, _CharT, _Traits, _Alloc> ’ is not derived from ‘const std::basic_string’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5916:5: note: candidate: template<_CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc> std::basic_string std::operator+(const std::basic_string &, const _CharT*)
operator+(const basic_string<_CharT, _Traits, _Alloc> & __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5916:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   ‘const std::map<std::string, int, _CharT, _Traits, _Alloc> ’ is not derived from ‘const std::basic_string’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:53:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.tcc:1173:5: note: candidate: template<_CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc> std::basic_string std::operator+(_CharT, const std::basic_string<_CharT, _Traits, _Alloc> &)
operator+(_CharT __lhs, const basic_string<_CharT, _Traits, _Alloc> & __rhs)
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.tcc:1173:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   mismatched types ‘const std::basic_string<_CharT, _Traits, _Alloc> ’ and ‘int’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:53:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.tcc:1157:5: note: candidate: template<_CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc> std::basic_string std::operator+(const _CharT*, const std::basic_string<_CharT, _Traits, _Alloc> &)
     operator+(const _CharT* __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.tcc:1157:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   mismatched types ‘const _CharT*’ and ‘std::map<std::string, int> ’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5879:5: note: candidate: template<_CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc, _CharT, _Traits, _Alloc> std::basic_string std::operator+(const std::basic_string &, const std::basic_string<_CharT, _Traits, _Alloc> &)
operator+(const basic_string<_CharT, _Traits, _Alloc> & __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5879:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   ‘const std::map<std::string, int, _CharT, _Traits, _Alloc> ’ is not derived from ‘const std::basic_string’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/bits/stl_algobase.h:67:0,
                 from /usr/include/c++/7/bits/char_traits.h:39,
                 from /usr/include/c++/7/string:40,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/stl_iterator.h:1198:5: note: candidate: template<_Iterator, _IteratorL, _IteratorL> std::move_iterator std::operator+(typename std::move_iterator::difference_type, const std::move_iterator<_IteratorL> &)
operator+(typename move_iterator<_Iterator> ::difference_type __n,
     ^~~~~~~~
/usr/include/c++/7/bits/stl_iterator.h:1198:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   mismatched types ‘const std::move_iterator<_IteratorL> ’ and ‘int’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/bits/stl_algobase.h:67:0,
                 from /usr/include/c++/7/bits/char_traits.h:39,
                 from /usr/include/c++/7/string:40,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/stl_iterator.h:397:5: note: candidate: template<_Iterator, _Iterator, _Iterator> std::reverse_iterator std::operator+(typename std::reverse_iterator::difference_type, const std::reverse_iterator<_Iterator> &)
operator+(typename reverse_iterator<_Iterator> ::difference_type __n,
     ^~~~~~~~
/usr/include/c++/7/bits/stl_iterator.h:397:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   mismatched types ‘const std::reverse_iterator<_Iterator> ’ and ‘int’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/bits/stl_algobase.h:67:0,
                 from /usr/include/c++/7/bits/char_traits.h:39,
                 from /usr/include/c++/7/string:40,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/stl_iterator.h:969:5: note: candidate: template<_Iterator, _Container, _Iterator, _Container, _Iterator, _Container> __gnu_cxx::__normal_iterator __gnu_cxx::operator+(typename __gnu_cxx::__normal_iterator::difference_type, const __gnu_cxx::__normal_iterator<_Iterator, _Container> &)
operator+(typename __normal_iterator<_Iterator, _Container> ::difference_type
     ^~~~~~~~
/usr/include/c++/7/bits/stl_iterator.h:969:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   mismatched types ‘const __gnu_cxx::__normal_iterator<_Iterator, _Container> ’ and ‘int’
     return v + 1;
            ~~^~~
In file included from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:0:
../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp: In instantiation of ‘std::vector<NewT> fp::transform(F, const std::vector<Y, lambda(const auto:1316 &), std::string, int, std::string, int> &) [with F = ; T = std::map; U = std::map]’:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:35:52:   required from here
../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:38:9: error: invalid use of void expression
         out.push_back(f(x));
         ^~~
[2/4] Building CXX object src/include/cleantype/compile_time/CMakeFiles/cleantype_decipherer.dir/cleantype_decipherer.cpp.o
ninja: build stopped: subcommand failed.

Error log without decipher / GCC
********************************

> ninja
[1/2] Building CXX object src/include/cleantype/...ple.dir/example/cleantype_decipher_example.cpp.o
FAILED: src/include/cleantype/compile_time/CMakeFiles/cleantype_decipher_example.dir/example/cleantype_decipher_example.cpp.o
/usr/bin/c++  -DCLEANTYPE_COMPILETIME -I../third_party/FunctionalPlus/include -I../src/include -I/root/.conan/data/boost/1.68.0/conan/stable/package/d6cb600df9137cb7943d00f26e402be306584995/include -std=c++14 -MD -MT src/include/cleantype/compile_time/CMakeFiles/cleantype_decipher_example.dir/example/cleantype_decipher_example.cpp.o -MF src/include/cleantype/compile_time/CMakeFiles/cleantype_decipher_example.dir/example/cleantype_decipher_example.cpp.o.d -o src/include/cleantype/compile_time/CMakeFiles/cleantype_decipher_example.dir/example/cleantype_decipher_example.cpp.o -c ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp: In instantiation of ‘<lambda(const auto:1316&)> [with auto:1316 = std::map<std::__cxx11::basic_string<char>, int>]’:
../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:38:24:   required from ‘std::vector<NewT> fp::transform(F, const std::vector<Y>&) [with F = <lambda(const auto:1316&)>; T = std::map<std::__cxx11::basic_string<char>, int>; U = std::map<std::__cxx11::basic_string<char>, int>]’
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:35:52:   required from here
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: error: no match for ‘operator+’ (operand types are ‘const std::map<std::__cxx11::basic_string<char>, int>’ and ‘int’)
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/deque:64:0,
                 from ../third_party/FunctionalPlus/include/fplus/container_traits.hpp:10,
                 from ../third_party/FunctionalPlus/include/fplus/container_common.hpp:10,
                 from ../third_party/FunctionalPlus/include/fplus/fplus.hpp:11,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:11,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/stl_deque.h:374:5: note: candidate: template<class _Tp, class _Ref, class _Ptr> std::_Deque_iterator<_Tp, _Ref, _Ptr> std::operator+(std::ptrdiff_t, const std::_Deque_iterator<_Tp, _Ref, _Ptr>&)
     operator+(ptrdiff_t __n, const _Deque_iterator<_Tp, _Ref, _Ptr>& __x)
     ^~~~~~~~
/usr/include/c++/7/bits/stl_deque.h:374:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   mismatched types ‘const std::_Deque_iterator<_Tp, _Ref, _Ptr>’ and ‘int’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/vector:65:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:8,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/stl_bvector.h:387:3: note: candidate: std::_Bit_const_iterator std::operator+(std::ptrdiff_t, const std::_Bit_const_iterator&)
   operator+(ptrdiff_t __n, const _Bit_const_iterator& __x)
   ^~~~~~~~
/usr/include/c++/7/bits/stl_bvector.h:387:3: note:   no known conversion for argument 1 from ‘const std::map<std::__cxx11::basic_string<char>, int>’ to ‘std::ptrdiff_t {aka long int}’
/usr/include/c++/7/bits/stl_bvector.h:297:3: note: candidate: std::_Bit_iterator std::operator+(std::ptrdiff_t, const std::_Bit_iterator&)
   operator+(ptrdiff_t __n, const _Bit_iterator& __x)
   ^~~~~~~~
/usr/include/c++/7/bits/stl_bvector.h:297:3: note:   no known conversion for argument 1 from ‘const std::map<std::__cxx11::basic_string<char>, int>’ to ‘std::ptrdiff_t {aka long int}’
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5986:5: note: candidate: template<class _CharT, class _Traits, class _Alloc> std::__cxx11::basic_string<_CharT, _Traits, _Alloc> std::operator+(std::__cxx11::basic_string<_CharT, _Traits, _Alloc>&&, _CharT)
     operator+(basic_string<_CharT, _Traits, _Alloc>&& __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5986:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   types ‘std::__cxx11::basic_string<_CharT, _Traits, _Alloc>’ and ‘const std::map<std::__cxx11::basic_string<char>, int>’ have incompatible cv-qualifiers
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5980:5: note: candidate: template<class _CharT, class _Traits, class _Alloc> std::__cxx11::basic_string<_CharT, _Traits, _Alloc> std::operator+(std::__cxx11::basic_string<_CharT, _Traits, _Alloc>&&, const _CharT*)
     operator+(basic_string<_CharT, _Traits, _Alloc>&& __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5980:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   types ‘std::__cxx11::basic_string<_CharT, _Traits, _Alloc>’ and ‘const std::map<std::__cxx11::basic_string<char>, int>’ have incompatible cv-qualifiers
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5974:5: note: candidate: template<class _CharT, class _Traits, class _Alloc> std::__cxx11::basic_string<_CharT, _Traits, _Alloc> std::operator+(_CharT, std::__cxx11::basic_string<_CharT, _Traits, _Alloc>&&)
     operator+(_CharT __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5974:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   mismatched types ‘std::__cxx11::basic_string<_CharT, _Traits, _Alloc>’ and ‘int’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5968:5: note: candidate: template<class _CharT, class _Traits, class _Alloc> std::__cxx11::basic_string<_CharT, _Traits, _Alloc> std::operator+(const _CharT*, std::__cxx11::basic_string<_CharT, _Traits, _Alloc>&&)
     operator+(const _CharT* __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5968:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   mismatched types ‘const _CharT*’ and ‘std::map<std::__cxx11::basic_string<char>, int>’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5956:5: note: candidate: template<class _CharT, class _Traits, class _Alloc> std::__cxx11::basic_string<_CharT, _Traits, _Alloc> std::operator+(std::__cxx11::basic_string<_CharT, _Traits, _Alloc>&&, std::__cxx11::basic_string<_CharT, _Traits, _Alloc>&&)
     operator+(basic_string<_CharT, _Traits, _Alloc>&& __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5956:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   types ‘std::__cxx11::basic_string<_CharT, _Traits, _Alloc>’ and ‘const std::map<std::__cxx11::basic_string<char>, int>’ have incompatible cv-qualifiers
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5950:5: note: candidate: template<class _CharT, class _Traits, class _Alloc> std::__cxx11::basic_string<_CharT, _Traits, _Alloc> std::operator+(const std::__cxx11::basic_string<_CharT, _Traits, _Alloc>&, std::__cxx11::basic_string<_CharT, _Traits, _Alloc>&&)
     operator+(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5950:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   ‘const std::map<std::__cxx11::basic_string<char>, int>’ is not derived from ‘const std::__cxx11::basic_string<_CharT, _Traits, _Alloc>’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5944:5: note: candidate: template<class _CharT, class _Traits, class _Alloc> std::__cxx11::basic_string<_CharT, _Traits, _Alloc> std::operator+(std::__cxx11::basic_string<_CharT, _Traits, _Alloc>&&, const std::__cxx11::basic_string<_CharT, _Traits, _Alloc>&)
     operator+(basic_string<_CharT, _Traits, _Alloc>&& __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5944:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   types ‘std::__cxx11::basic_string<_CharT, _Traits, _Alloc>’ and ‘const std::map<std::__cxx11::basic_string<char>, int>’ have incompatible cv-qualifiers
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5932:5: note: candidate: template<class _CharT, class _Traits, class _Alloc> std::__cxx11::basic_string<_CharT, _Traits, _Alloc> std::operator+(const std::__cxx11::basic_string<_CharT, _Traits, _Alloc>&, _CharT)
     operator+(const basic_string<_CharT, _Traits, _Alloc>& __lhs, _CharT __rhs)
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5932:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   ‘const std::map<std::__cxx11::basic_string<char>, int>’ is not derived from ‘const std::__cxx11::basic_string<_CharT, _Traits, _Alloc>’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5916:5: note: candidate: template<class _CharT, class _Traits, class _Alloc> std::__cxx11::basic_string<_CharT, _Traits, _Alloc> std::operator+(const std::__cxx11::basic_string<_CharT, _Traits, _Alloc>&, const _CharT*)
     operator+(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5916:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   ‘const std::map<std::__cxx11::basic_string<char>, int>’ is not derived from ‘const std::__cxx11::basic_string<_CharT, _Traits, _Alloc>’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:53:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.tcc:1173:5: note: candidate: template<class _CharT, class _Traits, class _Alloc> std::__cxx11::basic_string<_CharT, _Traits, _Alloc> std::operator+(_CharT, const std::__cxx11::basic_string<_CharT, _Traits, _Alloc>&)
     operator+(_CharT __lhs, const basic_string<_CharT, _Traits, _Alloc>& __rhs)
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.tcc:1173:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   mismatched types ‘const std::__cxx11::basic_string<_CharT, _Traits, _Alloc>’ and ‘int’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:53:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.tcc:1157:5: note: candidate: template<class _CharT, class _Traits, class _Alloc> std::__cxx11::basic_string<_CharT, _Traits, _Alloc> std::operator+(const _CharT*, const std::__cxx11::basic_string<_CharT, _Traits, _Alloc>&)
     operator+(const _CharT* __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.tcc:1157:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   mismatched types ‘const _CharT*’ and ‘std::map<std::__cxx11::basic_string<char>, int>’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/string:52:0,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/basic_string.h:5879:5: note: candidate: template<class _CharT, class _Traits, class _Alloc> std::__cxx11::basic_string<_CharT, _Traits, _Alloc> std::operator+(const std::__cxx11::basic_string<_CharT, _Traits, _Alloc>&, const std::__cxx11::basic_string<_CharT, _Traits, _Alloc>&)
     operator+(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
     ^~~~~~~~
/usr/include/c++/7/bits/basic_string.h:5879:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   ‘const std::map<std::__cxx11::basic_string<char>, int>’ is not derived from ‘const std::__cxx11::basic_string<_CharT, _Traits, _Alloc>’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/bits/stl_algobase.h:67:0,
                 from /usr/include/c++/7/bits/char_traits.h:39,
                 from /usr/include/c++/7/string:40,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/stl_iterator.h:1198:5: note: candidate: template<class _Iterator> std::move_iterator<_IteratorL> std::operator+(typename std::move_iterator<_IteratorL>::difference_type, const std::move_iterator<_IteratorL>&)
     operator+(typename move_iterator<_Iterator>::difference_type __n,
     ^~~~~~~~
/usr/include/c++/7/bits/stl_iterator.h:1198:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   mismatched types ‘const std::move_iterator<_IteratorL>’ and ‘int’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/bits/stl_algobase.h:67:0,
                 from /usr/include/c++/7/bits/char_traits.h:39,
                 from /usr/include/c++/7/string:40,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/stl_iterator.h:397:5: note: candidate: template<class _Iterator> std::reverse_iterator<_Iterator> std::operator+(typename std::reverse_iterator<_Iterator>::difference_type, const std::reverse_iterator<_Iterator>&)
     operator+(typename reverse_iterator<_Iterator>::difference_type __n,
     ^~~~~~~~
/usr/include/c++/7/bits/stl_iterator.h:397:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   mismatched types ‘const std::reverse_iterator<_Iterator>’ and ‘int’
     return v + 1;
            ~~^~~
In file included from /usr/include/c++/7/bits/stl_algobase.h:67:0,
                 from /usr/include/c++/7/bits/char_traits.h:39,
                 from /usr/include/c++/7/string:40,
                 from ../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:7,
                 from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:
/usr/include/c++/7/bits/stl_iterator.h:969:5: note: candidate: template<class _Iterator, class _Container> __gnu_cxx::__normal_iterator<_Iterator, _Container> __gnu_cxx::operator+(typename __gnu_cxx::__normal_iterator<_Iterator, _Container>::difference_type, const __gnu_cxx::__normal_iterator<_Iterator, _Container>&)
     operator+(typename __normal_iterator<_Iterator, _Container>::difference_type
     ^~~~~~~~
/usr/include/c++/7/bits/stl_iterator.h:969:5: note:   template argument deduction/substitution failed:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:26:14: note:   mismatched types ‘const __gnu_cxx::__normal_iterator<_Iterator, _Container>’ and ‘int’
     return v + 1;
            ~~^~~
In file included from ../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:1:0:
../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp: In instantiation of ‘std::vector<NewT> fp::transform(F, const std::vector<Y>&) [with F = <lambda(const auto:1316&)>; T = std::map<std::__cxx11::basic_string<char>, int>; U = std::map<std::__cxx11::basic_string<char>, int>]’:
../src/include/cleantype/compile_time/example/cleantype_decipher_example.cpp:35:52:   required from here
../src/include/cleantype/details/fp_polyfill/fp_polyfill.hpp:38:9: error: invalid use of void expression
         out.push_back(f(x));
         ^~~
ninja: build stopped: subcommand failed.
*/