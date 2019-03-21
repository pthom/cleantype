
# Table of content
<table>
<tr><td>
<p style="text-align: left;">
   <a href="#CleanType-Readable-C++-type-introspection---Compiler-Decipherer">CleanType: Readable C++ type introspection - Compiler Decipherer</a><br/>
   <a href="#Installation-and-usage">Installation and usage</a><br/>
   <a href="#About-this-manual">About this manual</a><br/>
   <a href="#Readable-type-names-and-full-type-names">Readable type names and full type names</a><br/>
   <a href="#Decipher-compiler-output-and-identify-types-in-the-compiler-output">Decipher compiler output and identify types in the compiler output</a><br/>
   <a href="#Identify-the-auto-return-type-of-functions-and-functors">Identify the auto return type of functions and functors</a><br/>
   <a href="#Identify-the-signature-of-lambdas">Identify the signature of lambdas</a><br/>
   <a href="#Settings---configure-replacements-and-indentation">Settings - configure replacements and indentation</a><br/>
   <a href="#Compile-time-constexpr-type-names">Compile time constexpr type names</a><br/>
   <a href="#Decipher-range-v3-auto-types">Decipher range-v3 auto types</a><br/>
   <a href="#Cheat-sheet">Cheat sheet</a><br/>

</p>
</tr></td>
</table>



<img src="https://raw.githubusercontent.com/pthom/cleantype/master/resources/cleantype_logo_small.jpg" align="left" width="200">
<table>
    <tr>        
      <td>
        Travis <a href="https://travis-ci.org/pthom/cleantype/"> <img src="https://travis-ci.org/pthom/cleantype.svg?branch=master" /> </a>
     </td>
     <td>
        Windows <a href="https://ci.appveyor.com/project/pthom/cleantype"> <img src="https://ci.appveyor.com/api/projects/status/s6wmhg5q9l69w7aq/branch/master?svg=true" /> </a>
    </td>
     <td>
        License <a href="https://www.boost.org/LICENSE_1_0.txt"> <img src="https://img.shields.io/badge/license-boost%201.0-blue.svg" /> </a>
    </td>
    <td>
        Try this online!
        <a href="https://mybinder.org/v2/gh/pthom/cleantype/master?filepath=notebooks%2Fcleantype%2Fcleantype.ipynb"> 
            <img src="https://mybinder.org/badge_logo.svg" /> 
        </a>
    </td>
    <td>
        Demo on gitpod
        <a href="https://gitpod.io/#https://github.com/pthom/cleantype/blob/master/demos/demo_simple.cpp"> 
            <img src="resources/gitpod.jpg" width="50" /> 
        </a>
    </td>
    </tr>
</table> 





<a href="#Table-of-content"><img src="https://img.shields.io/badge/%3C%20top-E7E7E7.svg" align="right"></a>
# CleanType: Readable C++ type introspection - Compiler Decipherer



CleanType is a C++14 header only library which offer *readable* type names, with a *consistent naming scheme across compilers*, at *run-time* and *compile-time*. It can also output the *signature of lambda* functions, and the result type of any auto function.



<a href="#Table-of-content"><img src="https://img.shields.io/badge/%3C%20top-E7E7E7.svg" align="right"></a>
## TLDR: a quick example for the impatient reader

````cpp
#include <cleantype/cleantype.hpp>
#include <range/v3/all.hpp>

int main() {
  std::set<std::string> v { "Hello", "There"};

  // cleantype::full -> full type string (the output will depend on the compiler and on the libc version)
  std::cout << cleantype::full(v) << "\n";
  // => Outputs: std::set<std::__cxx11::basic_string<char>, std::less<std::__cxx11::basic_string<char>>, std::allocator<std::__cxx11::basic_string<char>>> &

  // cleantype::clean -> readable type string (this string is constant across compilers and libc versions)
  std::cout << cleantype::clean(v) << "\n";
  // Outputs: std::set<std::string> &

  // cleantype::show_details -> readable type string together with variable content
  std::cout << cleantype::show_details(v) << "\n";
  // => Outputs: std::set<std::string> & = [Hello, There]

  // cleantype::clean -> can automatically indent deeply nested types (this is optional)
  using namespace ranges;
  auto squares_take_10 =  view::for_each(view::ints(1), [](int x) { return ranges::yield(x * x); }) | view::take(10);
  std::cout << cleantype::clean(squares_take_10) << "\n";
  // => Outputs:
  // ranges::v3::detail::take_exactly_view_<
  //    ranges::v3::join_view<
  //        ranges::v3::transform_view<
  //            ranges::v3::iota_view<
  //                int,
  //                void
  //            >,
  //            (lambda at example.cpp:19:59)
  //        >,
  //        void
  //    >,
  //    false
  //> &


  // cleantype::lambda_clean -> readable signature for lambda functions (generic and non generic lambdas)
  auto add = [](auto a, auto b) { return a + b; };
  std::cout << cleantype::lambda_clean<std::string, char>(add) << "\n";
  // => Outputs: lambda: (std::string, char) -> std::string
}

````

<a href="https://asciinema.org/a/HDu34hBlRJ8lbHPgvEXvvIqmN?speed=2.8&t=14" target="_blank"><img src="https://asciinema.org/a/HDu34hBlRJ8lbHPgvEXvvIqmN.svg" width="300" align="right" style="padding-left: 10px"/></a>
The video to the right gives a 2 minutes overview of the library. This demo uses [cling](https://root.cern.ch/cling), a fully compliant C++14 REPL, and [asciinema](https://asciinema.org/), a terminal session recorder. You can pause at any time, and copy-paste samples from it. Its source code is [here](demos/demo_cling.cpp).


The included tool `ct_compiler_decipher` simplifies the template noise in your compiler output: just ` "|" (pipe)` your build tool to it.


Cleantype can be seeen as a developper friendly `typeid` alternative, and as a tool for those who are tired by the template noise in the compiler output.




#### Motivation

In C++, [typeid.name()](https://en.cppreference.com/w/cpp/language/typeid) is able to display the type of variables.
However it has several limitations: `const`, `volatile`, `&&`qualifiers are ignored; it cannot identify the signature of lambdas function, and last but not least, the returned name if often unreadable: for example `std::set<std::string>` becomes
 ````
 std::set<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >
 ````

This library tries to overcomes some of these limitations. It is composed mainly of C++11 / C++14 functions. It also contains some macros in order to be able to display rvalue reference type, as well as variables names. Macros are prepended with a prefix 'CT_'.

The returned types names should be equal (or at least similar) accross compilers.

#### Status

Note: this library is heavily [tested](https://github.com/pthom/cleantype/tree/master/src/tests), with clang, gcc and msvc. However, it is subject to quick evolution, and should be considered as a beta version.

#### More infos
See the [introductory blog post](http://code-ballads.net/cleantype/)











<a href="#Table-of-content"><img src="https://img.shields.io/badge/%3C%20top-E7E7E7.svg" align="right"></a>
# Installation and usage


* `cleantype`is a small header only library, so you just need to clone it and add it to your path. It works with C++ 14 and C++17; and is tested succesfully with gcc (>=5), clang (>=6), appleclang, and msvc (>= MSVC 2017).

Then, include [cleantype/cleantype.hpp](src/include/cleantype/cleantype.hpp) (this file includes a comprehensive API doc). Alternatively, you can use the "all in one version", which is comprised of [only one header file](include_all_in_one/include/cleantype/cleantype.hpp), inside the "include_all_in_one" folder.

_Notes:_ 
cleantype can be used as a standalone library. However, in order to use be able to read settings from a preference file, you will need [nlohmann/json](https://github.com/nlohmann/json). If you want to use this feature, define `CLEANTYPE_USE_NLOHMANN_JSON` before including cleantype.hpp and make sure that nlohmann/json.hpp is in your include path.

* `ct_compiler_decipher` is comprised of a single c++ file. It's compilation can be done via `make`
or via `$(CXX) -Isrc/include -Ithird_party/FunctionalPlus/include --std=c++14 src/tools/ct_compiler_decipher/ct_compiler_decipher.cpp -o ct_compiler_decipher`
















<a href="#Table-of-content"><img src="https://img.shields.io/badge/%3C%20top-E7E7E7.svg" align="right"></a>
# About this manual


This manual is fully interactive! It was written using [cling](https://root.cern.ch/cling), [xeus cling](https://xeus-cling.readthedocs.io/en/latest/) and [jupyter notebook](https://jupyter.org/). Cling enables a Read-Eval-Print-Loop (REPL) development mode with C++. This approach benefits a lot from having a good type introspection, which is the aim of this library. I wrote an article which is a ["hands-on session"](http://code-ballads.net/read-eval-print-loop-repl-in-c/) about this for those who are interested.

The code that you read in this manual is real live code that can be executed inside jupyter notebook. 
You can try it directly inside [binder](https://mybinder.org/): click on the "launch binder" at the top of this page.<br/>
Notes:
* Beware, it may require about 1 minute to load; but then you will be able to run the code live from your browser!
* Inside the notebook, click on the "Run" button in order to execute each cell (in order)
* You can modify and run the code as you desire inside binder!
* Note that there is a limitation in cling that *requires that you add two ";" after each lambda function definition*

The "#pragma cling add_include_path" is specific to cling. Beside this, everything is standard C++.


```c++
// Special macro to change the include path (this is required for the binder demo)
#pragma cling add_include_path("./include")
#include <cleantype/cleantype.hpp>

// The includes below are not required, they are just used for the purpose of this manual
#include <future> 
#include <numeric>
#include <fplus/fplus.hpp>
```


```c++
// This macro is used later in this manual in order to display commands and their results
#define run_show(...)                   \
{                                       \
    std::cout << #__VA_ARGS__ << "\n";  \
    std::cout << __VA_ARGS__ << "\n\n"; \
}
```














<a href="#Table-of-content"><img src="https://img.shields.io/badge/%3C%20top-E7E7E7.svg" align="right"></a>
# Readable type names and full type names


 ## Readable type names 
 
 #### Functions operating on types and variables
                
* `cleantype::clean<T...>()` is a function that will return a string containing
   a readable type, for a given type or pack of types.
   Use it with "cleantype::clean<decltype(var)>()"<br/>
   Note: It will add a reference by default so that "int v = 5; cleantype::clean(v)" will return "int&". 
   Use the macro CT_cleantype_clean() if you want to avoid this

* `cleantype::clean<T...>(t...)` is a an easier version, using an instance of the type.<br/>
    Notes:
     * It will add a reference. Use the macro CT_cleantype_clean() if you want to avoid this
     * It is not able to output correctly r-value references. For this, use `cleantype::clean<decltype(var)>()`

* `cleantype::show_details(T && v)` is a function that will return a string containing
   the readable type of a variable, as well as its content

* `CT_cleantype_clean(var)` is a macro that will also return the full type,
   but, it is able to also correctly display rvalue reference types.

*  `CT_show_details(var)` is a macro that will return a string containing the name,
   type and content of a variable (in this case, the underlying type of 'var'
   has to have an 'ostream & operator<<')

* `CT_show_details_cont` (macro) is a version of CT_show_details for complex containers
   like "std::map". "cont" stands for "container".
   
#### Functions operating on strings

* `cleantype::clean_typestring(type_names)` will clean a given (list of) type(s) given as a string
* `cleantype::indent_type_tree(type_names)` will present an indented view of a given (list of) type(s).

_Note_: by default, types will be presented with an indentation as soon as the type tree depth is > 3. 


### Examples


```c++
// Lets define a function with an auto return type: what is its return type?
auto my_range(int nb)
{
    std::list<int> l(nb);
    std::iota(l.begin(), l.end(), 1);
    return l;
}
auto v = my_range(5);
```


```c++
run_show(     cleantype::clean(v)                   )
run_show(     cleantype::clean<decltype(v)>()       )
run_show(     cleantype::show_details(v)            )
run_show(     CT_cleantype_clean(v)                 )
run_show(     CT_show_details(v)                    )
```

    cleantype::clean(v)
    std::list<int> &
    
    cleantype::clean<decltype(v)>()
    std::list<int>
    
    cleantype::show_details(v)
    std::list<int> & = [1, 2, 3, 4, 5]
    
    CT_cleantype_clean(v)
    std::list<int>
    
    CT_show_details(v)
    [std::list<int>] v = [1, 2, 3, 4, 5]
    


### Example with rvalue references
Below, an where a function receives value by r-value references and can display its type & value correctly.


```c++
{
    auto log_received = [](auto && v) {
        std::cout << cleantype::full<decltype(v)>() << std::endl;
        std::cout << CT_show_details(v) << "\n";
    };
    log_received(42);
}
```

    int &&
    [int &&] v = 42


### Examples with arguments pack


```c++
std::cout << cleantype::clean(1, "Hello") << std::endl;
std::cout << cleantype::clean<std::string, int, int &&, char &&>() << std::endl;
```

    int, char const(&) [6] 
    std::string, int, int &&, char &&


### Example with strings


```c++
{
    // clean_typestring
    run_show(  cleantype::clean_typestring("std::__cxx11::list<int, std::allocator<int> >")    );

    // indent_type_tree
    run_show(  cleantype::indent_type_tree("std::deque<std::pair<std::string, std::map<int, int>>>")    );
}
```

    cleantype::clean_typestring("std::__cxx11::list<int, std::allocator<int> >")
    std::list<int>
    
    cleantype::indent_type_tree("std::deque<std::pair<std::string, std::map<int, int>>>")
    std::deque<
        std::pair<
            std::string,
            std::map<
                int,
                int
            >
        >
    >
    


### Configuration of the clean types

You can customize the suppressions and replacements inside [cleantype/cleantype_configuration.hpp](src/include/cleantype/cleantype_configuration.hpp)




















<a href="#Table-of-content"><img src="https://img.shields.io/badge/%3C%20top-E7E7E7.svg" align="right"></a>
## Full type names

* `cleantype::full<T...>()` is a function that will return a string containing
   the full type. It also works with packs of types. Use it with "cleantype::full<decltype(var)>()"<br/>
   It will add a reference by default so that "int v = 5; cleantype::full(v)" will return "int&". 
   Use the macro CT_cleantype_full() if you want to avoid this

* `cleantype::full<T...>(t...)` is a an easier version, using an instance of the type.<br/>
   Notes:
     * It will add a reference by default so that
        int v = 5; cleantype::full(v) will return "int&"
        => use the macro CT_cleantype_full() if you want to avoid this
     * It is not able to output correctly r-value references
         For this, use `cleantype::full<decltype(var)>()`

* `cleantype::show_details_full(T && v)` is a function that will return a string containing
   the full type of a variable, as well as its content

* `CT_cleantype_full(var)` is a macro that will also return the full type,
   but, it is able to also correctly display rvalue reference types.

*  `CT_show_details_full(var)` is a macro that will return a string containing the name,
   type and content of a variable (in this case, the underlying type of 'var'
   has to have an 'ostream & operator<<')

* `CT_show_details_full_cont` is a version of CT_show_details_full for complex containers
   like "std::map". "cont" stands for "container".




```c++
{
    auto w = my_range(10);
    run_show(     cleantype::full(w)                         )
    run_show(     cleantype::full<decltype(w)>()             )
    run_show(     cleantype::show_details_full(w)            )
    run_show(     CT_cleantype_full(w)                       )
    run_show(     CT_show_details_full(w)                    )
}
```

    cleantype::full(w)
    std::__cxx11::list<int, std::allocator<int>> &
    
    cleantype::full<decltype(w)>()
    std::__cxx11::list<int, std::allocator<int>>
    
    cleantype::show_details_full(w)
    [std::__cxx11::list<int, std::allocator<int>> &] = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    
    CT_cleantype_full(w)
    std::__cxx11::list<int, std::allocator<int>>
    
    CT_show_details_full(w)
    [std::__cxx11::list<int, std::allocator<int>>] w = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    


### Full type versus readable type:
Full types can quickly become unreadable, especially with templated types, such as in the STL.


```c++
{
    std::set<std::string> my_set { "Hello", "There"};
    run_show(     cleantype::show_details_full(my_set)               )
    run_show(     cleantype::show_details(my_set)                    )
}
```

    cleantype::show_details_full(my_set)
    [std::set<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>> &] = [Hello, There]
    
    cleantype::show_details(my_set)
    std::set<std::string> & = [Hello, There]
    





















<a href="#Table-of-content"><img src="https://img.shields.io/badge/%3C%20top-E7E7E7.svg" align="right"></a>
## Display the content of complex containers


* `CT_show_details_cont` (macro) is a version of CT_show_details for complex containers
   like "std::map". "cont" stands for "container".
* `CT_show_details_full_cont` enables to display the full type and content

These version are required for certains more complex containers, like "std::map". "cont" stands for "container".


```c++
{
    std::map<std::string, int> my_map {{{"a", 1}, {"b", 2}, {"c", 3} }};
    run_show(     CT_show_details_cont(my_map)                    )
    run_show(     CT_show_details_full_cont(my_map)               )
}
```

    CT_show_details_cont(my_map)
    [std::map<std::string, int>] my_map = [(a, 1), (b, 2), (c, 3)]
    
    CT_show_details_full_cont(my_map)
    [std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, int, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, std::allocator<std::pair<const std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, int>>>] my_map = [(a, 1), (b, 2), (c, 3)]
    















<a href="#Table-of-content"><img src="https://img.shields.io/badge/%3C%20top-E7E7E7.svg" align="right"></a>
# Decipher compiler output and identify types in the compiler output

<a href="https://asciinema.org/a/227653?speed=8" target="_blank"><img src="https://asciinema.org/a/227653.svg" width="200" align="right" style="padding-left: 20px"/></a>
* `ct_compiler_decipher` is a tool that deciphers the compiler output and makes it more readable, especially when there are lots of templates
* `CT_compiler_log_type(T)` is a macro that will create an intentional compiler error whose intent is to display the type name of T. You can use it in conjunction with "ct_compiler_decipher".
* `CT_compiler_log_var_type` is a macro that will create an intentional compiler error whose intent is to display the type name of the variable var. You can use it in conjunction with "ct_compiler_decipher".






















<a href="#Table-of-content"><img src="https://img.shields.io/badge/%3C%20top-E7E7E7.svg" align="right"></a>
## Decipher the compiler output


#### Build `ct_compiler_decipher`
First let's build ct_compiler_decipher: it is composed of unique cpp file, so that it's compilation is extremely easy


```c++
#include "build_ct_tools.cpp"
make_ct_compiler_decipher();
```

    make 2>&1
    g++ -Isrc/include --std=c++14 src/tools/ct_compiler_decipher/ct_compiler_decipher.cpp -o ct_compiler_decipher


#### Sample code with an error for which we want to deciher the compiler output
The code below is intentionally bad, in order to get the compiler to spit incomprehensible template errors. Do not even try to make sense out of it :-)


```c++
std::string code =
R"CODE(
 #include <fplus/fplus.hpp>
 #include <string>
 #include <map>
 #include <vector>

auto buggy_lambda = [](int a, int b) {
    std::map<std::string, int> r1;
    std::vector<decltype(r1)> r2;
    for (std::size_t i = 0; i < b; i++) r2.push_back(r1);
    auto add_one = [](auto x) { return x + 1; };
    auto r3 = fplus::transform(add_one, r2);
    return r3;
};

int main() { auto v = buggy_lambda(1, 3); }

)CODE";
```

#### Compile and decipher
Let's compile it with the default compiler. Below, we only show the begining of the compiler output (it is followed by around 90 similar lines).


```c++
compile_code__extract(code);
```

    clang++ --std=c++14 -c code.cpp -Iinclude -o a.out 2>&1 | head -5 2>&1
    code.cpp:10:42: error: invalid operands to binary expression ('std::map<std::__cxx11::basic_string<char>, int, std::less<std::__cxx11::basic_string<char> >, std::allocator<std::pair<const std::__cxx11::basic_string<char>, int> > >' and 'int')
        auto add_one = [](auto x) { return x + 1; };
                                           ~ ^ ~
    include/fplus/internal/invoke.hpp:211:26: note: in instantiation of function template specialization '(anonymous class)::operator()(int, int)::(anonymous class)::operator()<std::map<std::__cxx11::basic_string<char>, int, std::less<std::__cxx11::basic_string<char> >, std::allocator<std::pair<const std::__cxx11::basic_string<char>, int> > > >' requested here
        FPLUS_INVOKE_RETURN((std::forward<F>(f)(std::forward<Args>(args)...)))


Let's commpile it and pipe the compiler output to `ct_compiler_decipher`:
```bash
clang++ --std=c++14 -c code.cpp -Iinclude -o a.out 2>&1 | ct_compiler_decipher
```


```c++
compile_code_decipher__extract(code);
```

    clang++ --std=c++14 -c code.cpp -Iinclude -o a.out 2>&1 | ct_compiler_decipher | head -5 2>&1
    code.cpp:10:42: error: invalid operands to binary expression('std::map<std::string, int> ' and 'int')
        auto add_one = [](auto x) { return x + 1; };
                                           ~ ^ ~
    include/fplus/internal/invoke.hpp:211:26: note: in instantiation of function template specialization '(anonymous class)::operator()(int, int)::(anonymous class)::operator()<std::map<std::string, int>> ' requested here
    FPLUS_INVOKE_RETURN((std::forward<F, Args>(f)(std::forward(args)...)))





















<a href="#Table-of-content"><img src="https://img.shields.io/badge/%3C%20top-E7E7E7.svg" align="right"></a>
## Identify types names at compile time, with clean names


Sometimes it is easier to be able to identify a type at compile time. This is especially true, if the code in question is run long after the application start.

In this case, the macros `CT_compiler_log_type(T)` and `CT_compiler_log_var_type` come handy. They  will create an intentional compiler error whose intent is to display the type name of the variable var. You can use them in conjunction with "ct_compiler_decipher".

See an example below:


```c++
std::string code2 =
R"CODE(
 #include <cleantype/cleantype.hpp>
 #include <fplus/fplus.hpp>
 
 auto mystery_lambda = [](int end) {    
    return fplus::overlapping_pairs_cyclic( fplus::numbers(0, end) );
 };
 int main() {
    auto v = mystery_lambda(10);
    CT_compiler_log_var_type(v); // Here we ask the compiler to give us the type of v
 }
)CODE";
```


```c++
compile_code_decipher__extract(code2);
```

    clang++ --std=c++14 -c code.cpp -Iinclude -o a.out 2>&1 | ct_compiler_decipher | head -5 2>&1
    code.cpp:10:5: error: no member named 'IntentionalError' in 'std::vector<std::pair<int, int>> '
        CT_compiler_log_var_type(v); // Here we ask the compiler to give us the type of v
        ^                        ~
    include/cleantype/details/cleantype_full.hpp:103:13: note: expanded from macro 'CT_compiler_log_var_type'
            var.IntentionalError = 42;    \















<a href="#Table-of-content"><img src="https://img.shields.io/badge/%3C%20top-E7E7E7.svg" align="right"></a>
# Identify the auto return type of functions and functors


* `cleantype::invoke_result<F, Args...>::type` and `cleantype::invoke_result_t<F, Args...>` will contain the type of any function / lambda / functor. This is a __type__, _not a string_. 
* `CT_invoke_result_fn(F, Args...)` and `CT_invoke_result_fn_template(F, Args...)` are macros that makes it easy to get the return _type_ of a function whose return type is marked as "auto" (the second version is for template functions)
* `CT_type_fn`, `CT_type_fn_template`, `CT_type_fn_full` and `CT_type_fn_template_full` are macros that return a _string_ containing the return type of a function whose return type is marked as "auto"

__Notes:__
* "cleantype::invoke_result_t" is a C++14 polyfill for [`std::invoke_result`](https://en.cppreference.com/w/cpp/types/result_of) (C++14 only provides "std::result_of", which is to be deprecated soon). When using C++17, it uses std::invoke_result in the background.
* Yes, "CT_invoke_result_fn" is indeed a variadic macro!




















<a href="#Table-of-content"><img src="https://img.shields.io/badge/%3C%20top-E7E7E7.svg" align="right"></a>
## Examples


### Using invoke_result and invoke_result_t: get the return type as a type


```c++
namespace {
    auto add_auto_fn(int a, int b) { return a + b; }
    template<typename U, typename V> auto add_auto_template_fn(U a, V b) { return a + b; }
    struct my_functor{ auto operator() (int a) { return a + 42; }  };
}
void static_test_invoke_result()
{
    // For auto functions: use cleantype::invoke_result_t< decltype(&f), Args... >
    using T = cleantype::invoke_result_t< decltype(&add_auto_template_fn<int, double>), int, double>;
    static_assert(std::is_same<T, double>::value, "");

    // For templated auto functions: use cleantype::invoke_result_t< decltype(&f<Args...>), Args... >
    using U = cleantype::invoke_result_t< decltype(&add_auto_template_fn<int, double>), int, double>;
    static_assert(std::is_same<U, double>::value, "");

    // For generic lambdas: use cleantype::invoke_result_t< decltype(lambda), Args... >
    auto generic_lambda = [](auto a) {  return a + 42; };
    using V = cleantype::invoke_result_t< decltype(generic_lambda), double>;
    static_assert(std::is_same<V, double>::value, "");

    // For functors: use cleantype::invoke_result_t< functor, Args... >
    using W = cleantype::invoke_result_t< my_functor, int>;
    static_assert(std::is_same<W, int>::value, "");
}

```

### Get the return type as a string with `CT_type_fn`


```c++
// For auto functions: display their return type with CT_type_fn(f, Args...)
run_show (       CT_type_fn(add_auto_fn, int, int)                                   );

// For templated auto functions: display their return type with CT_type_fn_template(f, Args...)
run_show (       CT_type_fn_template(add_auto_template_fn, std::string, char)        );
```

    CT_type_fn(add_auto_fn, int, int)
    int
    
    CT_type_fn_template(add_auto_template_fn, std::string, char)
    std::string
    


__Limitations of invoke_result with MSVC 2017 and templated auto functions__:

`invoke_result` does not work under MSVC with template functions whose return type is auto (see https://stackoverflow.com/questions/54111146/invoke-result-for-template-function-with-auto-return-type-and-msvc-2017)














<a href="#Table-of-content"><img src="https://img.shields.io/badge/%3C%20top-E7E7E7.svg" align="right"></a>
# Identify the signature of lambdas





















<a href="#Table-of-content"><img src="https://img.shields.io/badge/%3C%20top-E7E7E7.svg" align="right"></a>
## Non generic lambdas

* `cleantype::lambda<typename... Args, typename Lambda>(Lambda fn, bool flag_clean)` is a function that will return
    a string containing the signature of a lambda. flag_clean controls wether the signature is cleaned or not.

* `cleantype::lambda_clean<typename... Args, typename Lambda>(Lambda fn)` is a function that will return a string containing the readable signature of a lambda.

* `cleantype::lambda_full<typename... Args, typename Lambda>(Lambda fn)` is a function that will return a string containing the full signature of a lambda

*  `CT_show_details_lambda(var)` is a macro that will return a string containing the
   readable signature of a lambda and its name

*  `CT_show_details_lambda_full(var)` is a macro that will return a string containing the
   full signature of a lambda and its name

It is not alway easy to guess the return type of lambda. See the lambda below for example. 
<br/>Its return type is not easy to guess:


```c++
int start = 5;
// what is the return type of this lambda?
auto mystery_lambda = [&start](int end) {    
    return fplus::overlapping_pairs_cyclic( fplus::numbers(start, end) );
};;
```


```c++
// Let's see
run_show(            cleantype::lambda_clean(mystery_lambda)          );
run_show(            CT_show_details_lambda(mystery_lambda)           );
```

    cleantype::lambda_clean(mystery_lambda)
    lambda: (int) -> std::vector<std::pair<int, int>>
    
    CT_show_details_lambda(mystery_lambda)
    [lambda: (int) -> std::vector<std::pair<int, int>>] mystery_lambda
    



#### Note
If we try to get the type of this lambda via `cleantype::full`, we do not get much information...


```c++
std::cout << cleantype::full<decltype(mystery_lambda)>();
```

    (lambda at input_line_29:4:23)

This is because "mystery_lambda" is actually a instance of a hidden class. We are actually looking for the signature of the operator() of this class. `type_lambda_clean` is able to extract the type of this operator and to display it in a readable way.




















<a href="#Table-of-content"><img src="https://img.shields.io/badge/%3C%20top-E7E7E7.svg" align="right"></a>
## Generic lambdas


* `cleantype::lambda`, `cleantype::lambda_clean` and `cleantype::lambda_full` are compatible with generic lambdas, provided that you specify the type of the "auto" params during the call.
*  `CT_type_lambda_generic_fromparams_XXX(lambda, arg1, arg2, ...)` is a macro that will return a string containing the signature of a generic lambda where you do not specify the args type, instead you give example of these types.
    (XXX is the number of params of the lambda, and can vary from 1 to 5).

Note: getting the signature of generic lambdas is an advanced feature, and it might fail on certain compilers, such as gcc (in which case, you will get no output).

### Example


```c++
auto add = [](auto a, auto b) {
    return a + b; 
};;
```

This lambda ("add") is a generic lambda (which means that at least one of its argument is specified with an "auto" type). It's behaviour is comparable to a template function.

So, if we try to get its signature via a direct call to `cleantype::lambda_clean` like below:

```cpp
std::cout << cleantype::lambda_clean(add) << std::endl;
```
The compiler will complain:
```
cleantype/details/cleantype_lambda.hpp:29:18: error: variable 'as_ptr' with type 'auto' has incompatible initializer of type '<overloaded function type>'
   auto as_ptr = &Lambda::operator(); // if you have an error here, your lambda is generic! Add template params for its input types!
```

The solution is to provide the types of the input parameters, like below:


```c++
// extract the lambda type using actual types
std::cout << cleantype::lambda_clean<int, double>(add) << std::endl;
```

    lambda: (int, double) -> double


It can also be done by providing some example parameters: use `CT_type_lambda_generic_fromparams_XXX`, where X is the number of parameters of the lambda.


```c++
// extract the lambda type using example params
std::cout << CT_type_lambda_generic_fromparams_2(add, 1u, -2);
```

    lambda: (unsigned int, int) -> unsigned int

This second version is useful when you are lost in a forest of "auto" variables deep in the call stack, and you do not know the return type of the lambda, and you do not even know the type of the input parameters: in that case, if you have a working call example, then you can use it.














<a href="#Table-of-content"><img src="https://img.shields.io/badge/%3C%20top-E7E7E7.svg" align="right"></a>
# Settings - configure replacements and indentation


In order to configure the behavior of cleantype:

* Duplicate the file `.cleantype.json` at the root of the cleantype repository, and place it wherever in the hierarchy of the parent directories of your application. 
<br/>_Note_: you can also edit the example json file via [an interactive online editor](https://jsoneditoronline.org/?id=df38aa61554e4aad92883eaede62edc2)
* Edit this file as a normal json file
* Save this file in anywhere in the hierarchy of the parent directories of the execution directory.

The content of the pref file is self explanatory:

````json
{
    "force_east_const_": false,
    "indent_depth_limit": 3,
    "replacements_after_undesirable_node_extractions": {
        "basic_string<_CharT, _Traits, _Allocator>": "std::string",
        "std::basic_string<char>": "std::string"
    },
    "suppress_custom_": [
        " __ptr64"
    ],
    "suppress_extra_namespaces_": [
        "::__1",
        "::__cxx11"
    ],
    "suppress_extract_struct_class_": [
        "class ",
        "struct "
    ],
    "undesirable_type_nodes_": [
        "std::char_traits",
        "std::allocator",
        "std::less"
    ]
}

````

_Note:_ 
* the library [nlohmann/json](https://github.com/nlohmann/json) is required in order to read/save the prefs from a json file. 
* in order to use the settings feature, define `CLEANTYPE_USE_NLOHMANN_JSON` before including cleantype.hpp and make sure that `nlohmann/json.hpp` is in your include path (you can find a version of it at `third_party/nlohmann_json/nlohmann/json.hpp`)















<a href="#Table-of-content"><img src="https://img.shields.io/badge/%3C%20top-E7E7E7.svg" align="right"></a>
# Compile time constexpr type names


### Get the typename as a Boost.Hana string

* `cleantype::full_compiletime<T>()` will give you the full name of a type in the form of a Boost.Hana string.

**Note:** For this, you need to manually include "cleantype/cleantype_compiler_typename.hpp", and to have boost in your include path. The rest of the library (including ct_compiler_decipherer) can be compiled in a standalone way.


### Note about constexpr type names:

The basic idea of having compile time type name is to use `__PRETTY_FUNCTION__` as a way to get the type name in a constexpr way. The original idea comes from the [ctti library](https://github.com/Manu343726/ctti), and is also used inside Boost.Hana's [experimental/type_name.hpp](https://github.com/boostorg/hana/blob/master/include/boost/hana/experimental/type_name.hpp). 

This project goes beyond what was done in these projects, by adding a support for gcc and msvc (only clang was supported originaly).

Based on the work done during the development of this librayr, a [Pull Request](https://github.com/boostorg/hana/pull/432) was posted to Boost.Hana. It proposes to include the support of MSVC and GCC for boost/hana/experimental/type_name.hpp.


### Example


```c++
#include <cleantype/cleantype_compiler_typename.hpp>
{
    auto hana_type_string = cleantype::full_compiletime<std::pair<int, char>>();
    run_show(      cleantype::full(hana_type_string)      );
}
```

    cleantype::full(hana_type_string)
    boost::hana::string<'s', 't', 'd', ':', ':', 'p', 'a', 'i', 'r', '<', 'i', 'n', 't', ', ', ' ', 'c', 'h', 'a', 'r', '>'> &
    















<a href="#Table-of-content"><img src="https://img.shields.io/badge/%3C%20top-E7E7E7.svg" align="right"></a>
# Decipher range-v3 auto types


[range-v3](https://github.com/ericniebler/range-v3) is a library which contains the current state of the ranges proposal that will be included in C++ 20.
It allows very expressive code. However, the design of the library is based on a complex collection of types, so that most of the functions and variables can only be noted as `auto`.  

`cleantype` can help in deciphering what's going on with the types with range-v3: 

First, we include cleantype and the range-v3 library

_Note: the examples with ranges-v3 do not work currently in the interactive version on binder (due to a default in the compiler input path). Please install cleantype locally in order to tests them._


```c++
#pragma cling add_include_path("./include")
#include <cleantype/cleantype.hpp>
#include <range/v3/all.hpp>

#define run__show(...)                  \
{                                       \
    std::cout << #__VA_ARGS__ << "\n";  \
    std::cout << __VA_ARGS__ << "\n\n"; \
}
using namespace ranges;
```

Then we set the indent depth limit to 2, because indenting will make the types much easier to read, since `range-v3` uses long and deep nested types. 


```c++
cleantype::CleanConfiguration::GlobalConfig().indent_depth_limit = 2;
```

### Example of a view that return square numbers

Let's define a function that yields using `ranges::yield` and, then let's identify its return type.


```c++
auto square_yield_fn(int x) {
    return ranges::yield(x * x);
}
run__show(  CT_type_fn(square_yield_fn, int)  );
```

    CT_type_fn(square_yield_fn, int)
    ranges::v3::single_view<int>
    


Now, let's define a view that transform ints into squares. This view is lazy, and unlimited (it never ends). We then identify its type:


```c++
auto squares_view_fn = view::for_each(view::ints(1), square_yield_fn);
run__show(  cleantype::clean(squares_view_fn)  );
```

    cleantype::clean(squares_view_fn)
    ranges::v3::join_view<
        ranges::v3::transform_view<
            ranges::v3::iota_view<
                int,
                void
            >,
            ranges::v3::single_view<
                int
            > (*)(int)
        >,
        void
    > &
    



```c++
auto squares_fn_take_10 = squares_view_fn | view::take(10);
run__show(  cleantype::clean(squares_fn_take_10)  );
```

    cleantype::clean(squares_fn_take_10)
    ranges::v3::detail::take_exactly_view_<
        ranges::v3::join_view<
            ranges::v3::transform_view<
                ranges::v3::iota_view<
                    int,
                    void
                >,
                ranges::v3::single_view<
                    int
                > (*)(int)
            >,
            void
        >,
        false
    > &
    


### Example with range-v3 and lambdas:

Since lambda are actually anonymous structs, cleantype cannot disclose the signature of the inner lambda of a view that is contructed using a lambda. 


```c++
{
    using namespace ranges;
    
    auto square_yield_lambda = [](int x) {
        return yield(x * x);
    };
    run__show(  cleantype::lambda_clean(square_yield_lambda)  );
    auto squares_view_lambda = view::for_each(view::ints(1), square_yield_lambda);
    run__show(  cleantype::clean(squares_view_lambda)  );
}
```

    cleantype::lambda_clean(square_yield_lambda)
    lambda: (int) -> ranges::v3::single_view<int>
    
    cleantype::clean(squares_view_lambda)
    ranges::v3::join_view<
        ranges::v3::transform_view<
            ranges::v3::iota_view<
                int,
                void
            >,
            (lambda at input_line_44:5:32)
        >,
        void
    > &
    


In an ideal world, I would be interesting to be able to display the view type as below (but I'm afraid that the code in order to get to this might be intractable).

````
ranges::v3::join_view<
    ranges::v3::transform_view<
        ranges::v3::iota_view<
            int,
            void
        >,
        lambda: (int) -> ranges::v3::single_view<int>
    >,
    void
> &
````

Thus, it is advised to prefer "auto return functions" to lambdas when using range-v3 with cleantype.














<a href="#Table-of-content"><img src="https://img.shields.io/badge/%3C%20top-E7E7E7.svg" align="right"></a>
# Cheat sheet

[cleantype.hpp](https://github.com/pthom/cleantype/blob/master/src/include/cleantype/cleantype.hpp) offers a quick cheat sheet to all the functions and macros available.
