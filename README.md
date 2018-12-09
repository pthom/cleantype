
# `type_name` a developper friendly `typeid` header only alternative
Display **readable** types and contents for C++ variables and lambdas. 

<!--
![Build Status Travis](https://travis-ci.org/pthom/type_name.svg?branch=master)
![Build Status AppVeyor](https://ci.appveyor.com/api/projects/status/github/pthom/type-name)
-->

<table>
    <tr>
      <td>
        Travis <img src="https://travis-ci.org/pthom/type_name.svg?branch=master" />
     </td>
     <td>
        Windows <img src="https://ci.appveyor.com/api/projects/status/s6wmhg5q9l69w7aq/branch/master?svg=true" />
    </td>
     <td>
        License <img src="https://img.shields.io/badge/license-boost%201.0-blue.svg" />
    </td>
    <td>
        Try this online!
        <a href="https://mybinder.org/v2/gh/pthom/type_name/typename?filepath=notebooks%2Ftypename%2Ftypename.ipynb"> 
            <img src="https://mybinder.org/badge_logo.svg" /> 
        </a>
    </tr>
</table>    

In C++, [typeid.name()](https://en.cppreference.com/w/cpp/language/typeid) is able to display the type of variables.
However it has several limitations:

* `const`, `volatile`, `&&`qualifiers are ignored
* if cannot identify the signature of lambdas functions
+ it cannot identify the return type of a function with an auto specifier
* the returned name if often unreadable : for example `std::set<std::string>` becomes
 ````
 std::set<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >
 ````
 

This library tries to overcomes some of these limitations. It is composed mainly of C++11 / C++14 functions. It also contains some macros in order to be able to display rvalue reference type, as well as variables names. Macros are prepended with a suffix 'm_'.

# Installation and usage
`type_name`is a small header only library, so you just need to clone it and add it to your path.

Then, include `type_name/type_name.hpp`.

You can also try this directly inside [binder](https://mybinder.org/) : click on the "launch binder" at the top of this page. Beware, it require about 2 minutes to load; but then you will be able to run the code live from your browser!


```c++
// Special macro to change the include path (this is required for the binder demo)
#pragma cling add_include_path("./include")
#include <type_name/type_name.hpp>
// The includes below are not required, they are just used for the purpose of this manual
#include <future> 
#include <numeric>
#include <fplus/fplus.hpp>
```


```c++
// This macro is used later in this manual in order to display commands and their results
#define run_show(cmd) \
{  \
    std::cout << #cmd << "\n"; \
    std::cout << cmd << "\n\n"; \
}
```

# Friendly type names for variables (clean type, name and value)

 ## Readable type names 
                 
* `type_name::clean<T>()` is a function that will return a string containing
   the readable type of a variable.
* `type_name::show_details(T && v)` is a function that will return a string containing
   the readable type of a variable, as well as its content
* `tn_type_name_clean(var)` is a macro that will also return the full type,
   but, it is able to also correctly display rvalue reference types.
*  `tn_show_details(var)` is a macro that will return a string containing the name,
   type and content of a variable (in this case, the underlying type of 'var'
   has to have an 'ostream & operator<<')

### Examples


```c++
// Lets define a function with an auto return type : what is its return type?
auto my_range(int nb)
{
    std::list<int> l(nb);
    std::iota(l.begin(), l.end(), 1);
    return l;
}
auto v = my_range(5);
```


```c++
run_show(     type_name::clean<decltype(v)>()       )
run_show(     type_name::show_details(v)            )
run_show(     tn_type_name_clean(v)                 )
run_show(     tn_show_details(v)                    )
```

    type_name::clean<decltype(v)>()
    std::list<int>
    
    type_name::show_details(v)
    std::list<int> & = [1, 2, 3, 4, 5]
    
    tn_type_name_clean(v)
    std::list<int>
    
    tn_show_details(v)
    [std::list<int>] v = [1, 2, 3, 4, 5]
    


## Full type names
* `type_name::full<T>()` is a function that will return a string containing
   the full type of a variable.
* `type_name::show_details_full(T && v)` is a function that will return a string containing
   the full type of a variable, as well as its content
* `tn_type_name_full(var)` is a macro that will also return the full type,
   but, it is able to also correctly display rvalue reference types.
*  `tn_show_details_full(var)` is a macro that will return a string containing the name,
   type and content of a variable (in this case, the underlying type of 'var'
   has to have an 'ostream & operator<<')



```c++
run_show(     type_name::full<decltype(v)>()             )
run_show(     type_name::show_details_full(v)            )
run_show(     tn_type_name_full(v)                       )
run_show(     tn_show_details_full(v)                    )
```

    type_name::full<decltype(v)>()
    std::__cxx11::list<int, std::allocator<int> >
    
    type_name::show_details_full(v)
    [std::__cxx11::list<int, std::allocator<int> >&] = [1, 2, 3, 4, 5]
    
    tn_type_name_full(v)
    std::__cxx11::list<int, std::allocator<int> >
    
    tn_show_details_full(v)
    [std::__cxx11::list<int, std::allocator<int> >] v = [1, 2, 3, 4, 5]
    


### Full type versus readable type :
Full types can quickly become unreadable, especially with templated types, such as in the STL.


```c++
std::set<std::string> my_set { "Hello", "There"};
run_show(     type_name::show_details_full(my_set)               )
run_show(     type_name::show_details(my_set)                    )
```

    type_name::show_details_full(my_set)
    [std::set<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >&] = [Hello, There]
    
    type_name::show_details(my_set)
    std::set<std::string> & = [Hello, There]
    


## Display the content of complex containers

* `tn_show_details_cont` (macro) is a version of tn_show_details for complex containers
   like "std::map". "cont" stands for "container".
* `tn_show_details_full_cont` enables to display the full type and content

These version are required for certains more complex containers, like "std::map". "cont" stands for "container".


```c++
std::map<std::string, int> my_map {{{"a", 1}, {"b", 2}, {"c", 3} }};
run_show(     tn_show_details_cont(my_map)                    )
run_show(     tn_show_details_full_cont(my_map)               )
```

    tn_show_details_cont(my_map)
    [std::map<std::string, int>] my_map = [(a, 1), (b, 2), (c, 3)]
    
    tn_show_details_full_cont(my_map)
    [std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, int> > >] my_map = [(a, 1), (b, 2), (c, 3)]
    


# Identify the signature of non generic lambdas

* `type_name::lambda_clean<LambdaFn>(LambdaFn fn)` is a function that will return a string containing
   the readable signature of a non generic lambda
* `type_name::lambda_clean<LambdaFn>(LambdaFn fn)` is a function that will return a string containing
   the full signature of a non generic lambda

*  `tn_show_details_lambda(var)` is a macro that will return a string containing the
   readable signature of a lambda and its name

*  `tn_show_details_lambda_full(var)` is a macro that will return a string containing the
   full signature of a lambda and its name

It is not alway easy to guess the return type of lambda. See the lambda below for example : it's return type is not easy to guess:


```c++
int start = 5;
// what is the return type of this lambda ?
auto mystery_lambda = [&start](int end) {    
    return fplus::overlapping_pairs_cyclic( fplus::numbers(start, end) );
};;
```


```c++
// Let's see
run_show(            type_name::lambda_clean(mystery_lambda)          );
run_show(            tn_show_details_lambda(mystery_lambda)           );
```

    type_name::lambda_clean(mystery_lambda)
    lambda: (int) -> std::vector<std::pair<int, int>>
    
    tn_show_details_lambda(mystery_lambda)
    [lambda: (int) -> std::vector<std::pair<int, int>>] mystery_lambda
    



#### Note
If we try to get the type of this lambda via `type_name_full`, we do not get much information...


```c++
std::cout << type_name::full<decltype(mystery_lambda)>();
```

    __cling_Un1Qu37(void*)::$_0

This is because "mystery_lambda" is actually a instance of a hidden class. We are actually looking for the signature of the operator() of this class. `type_lambda_clean` is able to extract the type of this operator and to display it in a readable way.

### Just for fun (sic...), a comparison of `clean` vs `full` lambda type


```c++
{
auto f = [](const std::set<std::string> & v) {
    return  std::make_pair(
        *v.begin(),
        v.size()
        );
};

std::cout << "(Clean) - " << show_details_lambda(f) << "\n\n";
std::cout << "(Full)  - " << show_details_lambda_full(f) << std::endl;
}
```

    (Clean) - [lambda: (std::set<std::string> const &) -> std::pair<std::string, unsigned long>] f
    
    (Full)  - [lambda: (std::set<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > const&) -> std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, unsigned long> ] f


# Identify the signature of generic lambdas

* `type_name::lambda_generic_clean<Args...>(LambdaFn fn)` is a function that will return a string containing
   the readable signature of a generic lambda. You will need to specify the type of the auto parameters
* `type_name::lambda_generic_full<Args...>(LambdaFn fn)` is a function that will return a string containing
   the full signature of a generic lambda.
*  `tn_type_lamda_generic_fromparams_XXX(lambda, arg1, arg2, ...)` is a macro that will return a string containing the
    signature of a generic lambda where you do not specify the args type, instead you give example of these types.
    (XXX is the number of params of the lambda, and can vary from 1 to 5).

Note: this is an advanced feature, and it might fail on certain compilers, such as gcc (in which case, you will get no output).

### Example


```c++
auto add = [](auto a, auto b) {
    return a + b; 
};;
```

This lambda ("add") is a generic lambda (which means that at least one of its argument is specified with an "auto" type). It's behaviour is comparable to a template function.
So, if we try to get the signature via "type_name::lambda_clean" we will run into an error. The solution is to provide the types of the input parameters:


```c++
// extract the lambda type using actual types
std::cout << type_name::lambda_generic_clean<int, double>(add) << std::endl;
```

    lambda: (int, double) -> double


It can also be done by providing some example parameters : use `tn_type_lamda_generic_fromparams_XXX`, where X is the number of parameters of the lambda.


```c++
// extract the lambda type using example params
std::cout << tn_type_lamda_generic_fromparams_2(add, 1u, -2);
```

    lambda: (unsigned int, int) -> unsigned int

This second version is useful when you are lost in a forest of "auto" variables deep in the call stack, and you do not know the return type of the lambda, and you do not even know the type of the input parameters: in that case, if you have a working call example, then you can use it.

### A more contrieved generic lambda example


```c++
auto generic_lambda = [](auto b) {
  std::vector<int> range = fplus::numbers(0, b);
  return fplus::pairs_to_map_grouped( fplus::overlapping_pairs_cyclic(range) );
};
std::cout << type_name::lambda_generic_clean<int>(generic_lambda);
```

    lambda: (int) -> std::map<int, std::vector<int>>

# Identify the return type of an `auto` function

Work in progress...


```c++
auto auto_fonction(int b) {
  std::vector<int> range = fplus::numbers(0, b);
  return fplus::pairs_to_map_grouped( fplus::overlapping_pairs_cyclic(range) );
};
```

# The zoo of type qualifiers
`type_name`handles quite well `const`, `volatile`, references (`&`), rvalue references (`&&`), and pointers (`*`). See below a demonstration 


```c++
auto foo = [](auto && x) {
    run_show(     tn_type_name_clean(x)                                );
    run_show(     type_name::clean<decltype(x)>()                      );
};;

#define test_call_foo(var, title)                                        \
    std::cout << " *********** " << title << " ***********\n";           \
    std::cout << "Bare variable (before the call)\n";                    \
    run_show(     type_name::clean<decltype(var)>()                    );\
    run_show(     tn_type_name_clean(var)                              );\
    std::cout << "Now inside foo (pass by universal reference)\n";       \
    foo(var);
```


```c++
{
    int a = 5;  
    test_call_foo(a, "Calling with an int lvalue (transformed to a ref by the call)");
    
    int const &  b = a; 
    test_call_foo(b, "Calling with a const lvalue reference");
    
    test_call_foo(42, "Calling with an rvalue reference");
    
    int * const c = &a;
    test_call_foo(c, "Calling with a const pointer");

    int const * d = &a;
    test_call_foo(d, "Calling with a pointer to const");

    int volatile vol = 42;
    test_call_foo(vol, "Calling with a volatile int");
    
    int const e = 5;  
    test_call_foo(e, "Calling with a const");

}
```

     *********** Calling with an int lvalue (transformed to a ref by the call) ***********
    Bare variable (before the call)
    type_name::clean<decltype(a)>()
    int
    
    tn_type_name_clean(a)
    int
    
    Now inside foo (pass by universal reference)
    tn_type_name_clean(x)
    int &
    
    type_name::clean<decltype(x)>()
    int &
    
     *********** Calling with a const lvalue reference ***********
    Bare variable (before the call)
    type_name::clean<decltype(b)>()
    int const &
    
    tn_type_name_clean(b)
    int const &
    
    Now inside foo (pass by universal reference)
    tn_type_name_clean(x)
    int const &
    
    type_name::clean<decltype(x)>()
    int const &
    
     *********** Calling with an rvalue reference ***********
    Bare variable (before the call)
    type_name::clean<decltype(42)>()
    int
    
    tn_type_name_clean(42)
    int
    
    Now inside foo (pass by universal reference)
    tn_type_name_clean(x)
    int &&
    
    type_name::clean<decltype(x)>()
    int &&
    
     *********** Calling with a const pointer ***********
    Bare variable (before the call)
    type_name::clean<decltype(c)>()
    int* const
    
    tn_type_name_clean(c)
    int* const
    
    Now inside foo (pass by universal reference)
    tn_type_name_clean(x)
    int* const &
    
    type_name::clean<decltype(x)>()
    int* const &
    
     *********** Calling with a pointer to const ***********
    Bare variable (before the call)
    type_name::clean<decltype(d)>()
    int const*
    
    tn_type_name_clean(d)
    int const*
    
    Now inside foo (pass by universal reference)
    tn_type_name_clean(x)
    int const* &
    
    type_name::clean<decltype(x)>()
    int const* &
    
     *********** Calling with a volatile int ***********
    Bare variable (before the call)
    type_name::clean<decltype(vol)>()
    int volatile
    
    tn_type_name_clean(vol)
    int volatile
    
    Now inside foo (pass by universal reference)
    tn_type_name_clean(x)
    int volatile &
    
    type_name::clean<decltype(x)>()
    int volatile &
    
     *********** Calling with a const ***********
    Bare variable (before the call)
    type_name::clean<decltype(e)>()
    int const
    
    tn_type_name_clean(e)
    int const
    
    Now inside foo (pass by universal reference)
    tn_type_name_clean(x)
    int const &
    
    type_name::clean<decltype(x)>()
    int const &
    

