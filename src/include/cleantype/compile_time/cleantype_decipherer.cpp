#include <cleantype/details/fp_polyfill/fp_additions.hpp>
#include <cleantype/cleantype.hpp>


std::string decipher_line(const std::string & compiler_line)
{
    bool needs_decipher = true;
    if (needs_decipher)
        return cleantype::internal::impl_clean_several_types(compiler_line);
    else
        return compiler_line;
}

int main()
{
    auto prog = fp::interact_by_line(decipher_line);
    prog();
}

/*

Error log with decipher :
*************************

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