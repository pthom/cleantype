
* [ ] fplus
   * [ ] polyfill show_cont / remove include
   * [ ] string_tree is not needed -> remove dupl
   * [ ] post suggestion / additions
* [ ] code-ballads
   * [ ] Blog wordpress ?
* [ ] Doc
    * [ ] Remove zoo qualifiers (or shorten it)
    * [ ] Add remarks / boost::hana & ctti
    * [ ] Mention look for help?
    * [ ] Article ?
    * [ ] Insertion dans ClingRepl ?


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
