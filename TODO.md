* [X] Cleanup
    * [X] Indent : no \n before <
    * [X] Link article / REPL C++
    * [X] Patience for interactive demo
    * [X] Shorten article so that manual is reached
    * [X] Demo indent / no indent
    * [X] Proof read & classify code
    * [X] Doc inside cleantype.hpp (missing invoke, etc)
    * [X] debug.hpp / json.hpp
    * [X] Configuration via pref file ?
    * [X] third parties cleanup
    * [X] Continuous Integration : C++14 & C++17
    * [X] Doc
        * [X] Doc reorder
        * [X] Doc : example hana static string + example type
        * [X] State alpha -> apha / beta
        * [X] Change link "Note: this library is heavily tested"
        * [X] Layout doc : Table of Content?
        * [X] Add example with range v3 (no submodule : checkout via notebook)
        * [X] Better doc / config

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
        * [-] PR hana_pr_typename
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
