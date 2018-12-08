
# Display friendly type names
Trying to solve the sad state of variables introspection at runtime: display a clean type, introspect lambda params and return types, etc.

Using a homemade library (https://github.com/pthom/type_name). 


```c++
#pragma cling add_include_path("../../external/type_name/src/include")
#pragma cling add_include_path("../../external/FunctionalPlus/include")
#include <type_name/type_name_clean.hpp>
#include <type_name/details/lambda_typename.hpp>
#include <fplus/fplus.hpp>
```

# Friendly names for variables (clean type, name and value)

## Basic usage
### Standard variables
Simply call `log_var(var)`, and you will see the type, name and content of the variable !


```c++
std::vector<int> values {1, 2, 3};
log_var(values);
int a = 5;
log_var(a);
```

    [std::vector<int>] values = [1, 2, 3]
    [int] a = 5


### Complex containers
For certains containers (like `std::map`), you need to call `log_var_cont`. `cont` stands for `container`


```c++
std::map<std::string, int> my_map {{{"a", 1}, {"b", 2}, {"c", 3} }};
log_var_cont(my_map);
```

    [std::map<std::string, int>] my_map = [(a, 1), (b, 2), (c, 3)]


## Comparative demo with different types
Here we compare the full type with the "cleaned" version:


```c++
#define log_clean_vs_full(var)          \
std::cout << "(Clean) - "; log_var(var);  \
std::cout << "(Full)  - "; log_var_full(var);

#define log_clean_vs_full_cont(var)          \
std::cout << "(Clean) - "; log_var_cont(var);  \
std::cout << "(Full)  - "; log_var_full_cont(var);

```


```c++
{
int a = 1;
log_clean_vs_full(a);
}
```

    (Clean) - [int] a = 1
    (Full)  - [int] a = 1



```c++
{
std::string v("hello");
log_clean_vs_full(v);
}
```

    (Clean) - [std::string] v = hello
    (Full)  - [std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >] v = hello



```c++
{
std::vector<int> v{{ 1, 2, 3, 4, 5}};
log_clean_vs_full(v);
}
```

    (Clean) - [std::vector<int>] v = [1, 2, 3, 4, 5]
    (Full)  - [std::vector<int, std::allocator<int> >] v = [1, 2, 3, 4, 5]



```c++
{
std::map<std::string, int> v {{
	{"a", 1},
	{"b", 2},
	{"c", 3}
}};
log_clean_vs_full_cont(v);
}
```

    (Clean) - [std::map<std::string, int>] v = [(a, 1), (b, 2), (c, 3)]
    (Full)  - [std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, int> > >] v = [(a, 1), (b, 2), (c, 3)]



```c++
{
std::vector<std::string> v {{
	{"a"},
	{"b"},
	{"c"}
}};
log_clean_vs_full(v);
}
```

    (Clean) - [std::vector<std::string>] v = [a, b, c]
    (Full)  - [std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >] v = [a, b, c]



```c++
{
std::set<std::string> v {{
	{"a"},
	{"b"},
	{"c"}
}};
log_clean_vs_full(v);
}
```

    (Clean) - [std::set<std::string>] v = [a, b, c]
    (Full)  - [std::set<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >] v = [a, b, c]


# Log lambdas (params & return type + name)

Normaly, the inspection of a lambda at runtime reveals only a random name and address. 

Here, we demonstrate how we can still extract some info from lambda functions. 

For each of the lambdas below, the call to `log_type_lambda_clean` will output the correct lambda input params / return type.  


```c++
{
auto f = [](){ std::cout << "Hello"; };
log_type_lambda_clean(f);
}
```

    [lambda: () -> void] f



```c++
{
auto f = [](){ return 42u; };
log_type_lambda_clean(f);
}
```

    [lambda: () -> unsigned int] f



```c++
{
int c = 5;
auto f = [&c](int a, int b) -> double { return a + b + c; };
log_type_lambda_clean(f);
}
```

    [lambda: (int, int) -> double] f



```c++
{
int c = 5;
auto f = [](int a, int b)  { return std::pair<int, double>(a + b, cos(a + static_cast<double>(b))); };
log_type_lambda_clean(f);
}
```

    [lambda: (int, int) -> std::pair<int, double>] f



```c++
{
std::string prefix = "a-";
auto f = [&prefix](const std::string &s)  { return prefix + s; };
log_type_lambda_clean(f);
}
```

    [lambda: (std::string& const) -> std::string] f


### Just for fun (sic...), a comparison of `clean` vs `full` lambda type


```c++
{
auto f = [](const std::set<std::string> & v) {
    return  std::make_pair(
        *v.begin(),
        v.size()
        );
};

std::cout << "(Clean) - "; log_type_lambda_clean(f);
std::cout << "(Full)  - "; log_type_lambda_full(f);
}
```

    (Clean) - [lambda: (std::set const&<std::string>) -> std::pair<std::string, unsigned long>] f
    (Full)  - [lambda: (std::set<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > const&) -> std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, unsigned long> ] f

