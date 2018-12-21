#include <cleantype/details/fp_polyfill/fp_additions.hpp>
#include <cleantype/cleantype.hpp>
#include <sstream>


std::string msvc_error_log = R"(
)";

std::pair<int, int> count_templates(const std::string & compiler_line)
{
    std::pair<int, int> r;
    for (auto c : compiler_line)
    {
        if (c == '<')
        {
            r.first++;
            r.second++;
        }
        if (c == '>')
        {
            r.first++;
            r.second--;
        }
    }
    return r;
}

bool need_decipher_line(const std::string & compiler_line)
{
    auto r = count_templates(compiler_line);
    int total_open_close = r.first;
    int running_total = r.second;

    if (running_total != 0)
        return false;
    return (total_open_close >= 2);
}

std::string msvc_remove_false_open_template(const std::string & compiler_line) 
{
#ifdef _MSC_VER
    // msvc compile lines start with a >
    // example :
    // 1>F:\dvp\OpenSource\type_name\src\include\cleantype/details/fp_polyfill/fp_polyfill.hpp(38): n

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
        //return std::string("PROCESSED\n") + cleantype::internal::impl_clean_several_types(compiler_line_cut) + "\n\n";
        return cleantype::internal::impl_clean_several_types(compiler_line_cut);
    else
        //return std::string("BARE\n") + compiler_line + "\n\n";
        return compiler_line;
}

int main()
{
    auto prog = fp::interact_by_line(decipher_line);

    // Debug version
    //{
    //    std::istringstream is(msvc_error_log);
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