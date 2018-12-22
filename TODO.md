* [X]  type_name_s :
    * [X] hana
        * [X] pas de doctest, main only
        * [X] pas de format
        * [X] test / regex
        * [X] recup test dans full_s
        * [-] tenter PR dans branche hana_pr_typename
        * [X] le code principal doit fonctionner sans hana, donc sans namespace hana
                -> faire dupl...
    * [X] Types multiples par wrap dans Template Holder ?
    * [X] full_s : compile time tests
    * [X] west_const -> east_const
    * [X] suppr typename, replace by type_name_s
    * [X] retest lambda and 4 params types (was an issue, should not be anymore) : see cleantype_full_multiple_fromvalues
    * [X] clean : check issues with multiple types (why parameter packs...)
    * [X] impl_clean_several_types -> publier dans API (et renommer)
    * [ ] Compile time types log
        * [X] CMake / detection boost
        * [X] Header separe pour inclusion hana
        * [ ] interact / clean
            * [X] prog version inline
            * [X] command line options / clean
            * [X] On peut extraire le nom de la variable recherche en regardant quelques lignes plus bas,

    * [ ] Change macro names


    * [ ] decipherer
        * [X] rename
        * [X] install
        * [X] clean code
        * [ ] doc
        * [N] test unitaire avec output / avec erreur reelle
        * [ ] make version inline pour binder


    * [ ] utiliser fplus pour de bon ?

    * [X] Compiler log decipherer
        * [X] clang
        * [X] msvc
        * [X] gcc
        * [X] interact progressive ou utiliser tee ?


* [X] Passer le code en east const
* [X] includes avec <>
* [X] classer fonctions string dans stringutils
* [?] cleantype or ConsType ?

* [X] Change name lib : clean type ?
    * [X] Change copyright messsage & add one empty line

* [ ] string_tree : delete, only lhr_rhs
* [ ] fplus : remove include (i.e backport show_cont & co)
* [ ] Nettoyage code

* [ ] Doc
    * [ ] Mention look for help?
    * [ ] Article ?
    * [ ] Insertion dans ClingRepl ?
    * [ ] Mention hana & ctti

Bugs
    * [ ] type function / several params
    * [ ] Windows : disabled type function
    * [ ] lambda composed gcc ?

* [ ] hana notebooks based on examples?


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
