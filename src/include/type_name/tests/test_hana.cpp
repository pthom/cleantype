// #include <iostream>
// #include "doctest.h"
// #include <cassert>
// #include <iostream>
// #include <string>
// #include <boost/hana.hpp>
// #include <boost/hana/experimental/type_name.hpp>

// namespace hana = boost::hana;

// struct Fish { std::string name; };
// struct Cat  { std::string name; };
// struct Dog  { std::string name; };


// auto animals = hana::make_tuple(Fish{"Nemo"}, Cat{"Garfield"}, Dog{"Snoopy"});

// using namespace hana::literals;
// // Access tuple elements with operator[] instead of std::get.
// Cat garfield = animals[1_c];
// // Perform high level algorithms on tuples (this is like std::transform)
// auto names = hana::transform(animals, [](auto a) {
//   return a.name;
// });

// auto has_name = hana::is_valid([](auto&& x) -> decltype((void)x.name) { });
// static_assert(has_name(garfield), "");
// static_assert(! has_name(1), "Should have a name field !");



// template <char ...s>
// constexpr char const string_storage[sizeof...(s) + 1] = {s..., '\0'};

// template<typename T>
// void ERROR_typename_compile_time() {
//   // will make the compiler fail (no call operator), but you can read
//   // the name in the output if you squint your eyes
//   //constexpr auto t = boost::hana::experimental::type_name<T>()();
//   static_assert(boost::hana::experimental::type_name<T>() , "truc");
// }




// template<typename T> void tests() {
//   // constexpr auto str = hana::string_c<'h', 'i'>;
//   // constexpr auto c = str.c_str();

//   //constexpr auto t = boost::hana::experimental::type_name<T>()();
//   // constexpr auto s = t.c_str();

//   //constexpr auto v = boost::hana::experimental::type_name<int>().c_str();
//   // //static_assert(false, s_s);
//   // auto cc = hana::unpack(t, hana::plus);

//   {
//     //boost::hana::string s("Hello");
//     //hana::string s("a");
//     auto s = BOOST_HANA_STRING("abcdef");
//     //constexpr auto sss = string_storage<s>();
//     //static_assert(false, sss);
//     //#pragma warning(sss)

//     //auto s2 = string_storage<'a', 'b', 'c'>;
//     //constexpr auto sss = string_storage<s2>();
//     //static_assert(false, s2);
//     //static_assert(false, s2);
//     //#pragma message(s2);
//   }

// }


// #define LOG(w) std::cout << w << std::endl;

// #include <vector>
// #include <string>

// TEST_CASE("test_hana")
// {
//   //ERROR_typename_compile_time< int >();
// }