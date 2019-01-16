* [ ] Cleanup
    * [X] Configuration via pref file ?
        * [ ] ? Change path for nlohmann/json : do not replace other instances (or do ?) !
    * [ ] third parties cleanup:
        * [ ] Separate unit test & doc third parties from required third parties
        * [ ] Remove submodules used only for tests?
        * [ ] Document install & third parties
        * [ ] Use Fplus instead of polyfills ?
            * [X] Compat bare fplus
    * [ ] Continuous Integration : C++14 & C++17
    * [ ] Doc
        * [ ] Range-v3 : will be part of C++20 (not probably) (verif sur blog e niebler)
        * [X] Layout doc : Table of Content?
        * [X] Add example with range v3 (no submodule : checkout via notebook)
        * [X] Better doc / config
        * [ ] Mention look for help?
        * [ ] Article ?
    * [ ] Contacts & RFC :
        * [ ] DH
        * [ ] EN

* [ ] Other
    * [ ] doc / anonmymous namespace for several functions (code ballads)
    * [ ] hana notebooks based on examples?

* [ ] FPlus :
    * [ ] Officialy use it as a submodule
    * [ ] Document this and advertise
    * [ ] string_tree : delete, only lhr_rhs
    * [ ] remove bakports
    * [ ] propose extensions


* [X] Range-v3 & indent:
    * [X] auto-indent clean
    * [X] pref / start auto indent
    * [X] doc / range-v3 & indent

* invoke_result :
    * [X] remove cleantype_function.hpp
    * [X] polyfill invoke_result
    * [N] invoke_result_clean / invoke_result_full
    * [X] CT_invoke_result_fn, CT_invoke_result_fn_template
    * [X] improve invoke_result on functions ant templated functions
    * [X] Doc (separate page ?)

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
    * [X] Change macro names
    * [X] clean : check issues with multiple types (why parameter packs...)
    * [X] impl_clean_several_types -> publier dans API (et renommer)
    * [X] Compile time types log
        * [X] CMake / detection boost
        * [X] Header separe pour inclusion hana
        * [X] interact / clean
            * [X] prog version inline
            * [X] command line options / clean
            * [X] On peut extraire le nom de la variable recherche en regardant quelques lignes plus bas,

    * [X] Notebook Doc
        * [X] decipherer build tool
        * [X] decipherer
        * [X] check status / references & update accordingly
        * [X] doc / build and usage decipherer


    * [X] Compiler log decipherer
        * [X] clang
        * [X] msvc
        * [X] gcc
        * [X] interact progressive ou utiliser tee ?

    * [X] east const

* [X] Check references: (same, and for compile time)
    * [X] boost::hana::experimental::type_name
    * [X] ctti : Compile Time Type Information for C++
