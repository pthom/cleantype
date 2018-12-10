
* [X] template <class T> std::string full(T)
    * [X] doc
* [X] <Args...> template <typename... Args> std::string full()
    * [X] doc

* [ ] Doc
    * [X] Script deploy(bash function)
    * [X] Mention beta
    * [ ] Mention look for help?
    * [ ] Article ?
    * [ ] Insertion dans ClingRepl ?

* [X] Windows build fails
    * [X] Investigate / Windows build issue

* [X] make replacements customizable
* [X] Make macros prefix configurable / impossible => switched to TN_



Bugs
    * [ ] type function / several params
    * [ ] Windows : disabled type function
    * [ ] lambda composed gcc ?
    * bug / 3rd param (forget this ?!)


* Check references: (same, and for compile time)
    * [X] boost::hana::experimental::type_name
        (-)
            http://boostorg.github.io/hana/group__group-experimental.html#gaf14876d1f1a3c42ce7a0243d7b263bec
            Pas de clean
            Rien sur les lambdas (a part No de ligne dans le cpp)
            Ok / types
        (+)
            OK / &, &&, const
            constexpr strings

    * [X] ctti : Compile Time Type Information for C++
        https://github.com/Manu343726/ctti
        (-)
            Clean pas terrible : ok pour string, pas ok pour set ni vector<int> !
            Gere pas les lambda / pas les fonction
        (+)
            Compile time
            Fournit ctti::detail::cstring , une string constexpr !
            OK / &, &&, const
            constexpr strings
