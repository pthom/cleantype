// Cave 48 / 3eme sous sol
// M. Hubuile
// 0652150221
// 9h
//

* [ ]  type_name_s :
    * [ ] hana
        * [X] pas de doctest, main only
        * [X] pas de format
        * [X] test / regex
        * [X] recup test dans full_s
        * [ ] tenter PR dans branche hana_pr_typename
    * [X] Types multiples par wrap dans Template Holder ?
    * [X] full_s : compile time tests
    * [ ] west_const -> east_const
    * [X] suppr typename, replace by type_name_s
    * [X] retest lambda and 4 params types (was an issue, should not be anymore) : see constype_full_multiple_fromvalues
    * [ ] clean : check issues with multiple types (why parameter packs...)
    * [ ] Compile time types log
        * [ ] CMake / detection boost
        * [ ] interact / clean
            * [ ] prog version inline
            * [ ] make version inline pour binder

* [ ] Passer le code en east const
* [ ] includes avec <>
* [ ] classer fonctions string dans stringutils
* [?] constype or ConsType ?
* [X] Trouver un bon nom pour la librairie : consistent type "constype"
 * [X] Change name
 * [ ] Add copyright

* [ ] string_tree : delete, only lhr_rhs
* [ ] fplus : remove include (i.e backport show_cont & co)

* [ ] Doc
    * [ ] Mention look for help?
    * [ ] Article ?
    * [ ] Insertion dans ClingRepl ?
    * [ ] Mention hana & ctti
    * [ ] Version compile time
        * [ ] Outil de compil
        * [ ] Outil de clean post compil

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
