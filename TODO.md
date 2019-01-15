* Range-v3 & indent:
    * [ ] auto-indent clean
    * [ ] pref / start auto indent
    * [ ] extract tupleholder to hpp

    inline std::string clean_typestring(const std::string & type_names)
    {
        return internal::impl_clean_several_types(type_names);
    }
    inline std::string impl_clean_several_types(std::string const & type_names)
    {
        std::vector<std::string> types = split_types(type_names, true);
        std::string r = fp::join(", ", types);
        return r;
    }

    Refac:
        clean_several_types : use strange impl_clean_one_type(bool remove_holder)
        split_types -> rename with type list in the name

* [ ] Cleanup
    * [ ] Remove submodule hana (checkout via notebook if needed)
    * [ ] Configuration via pref file ?
    * [ ] Doc
        * [ ] Layout doc : Table of Content?
        * [ ] Add example with range v3 (no submodule : checkout via notebook)
        * [ ] Better doc / config
        * [ ] Mention look for help?
        * [ ] Article ?
    * [ ] utiliser fplus pour de bon ?
    * [ ] Continuous Integration : C++14 & C++17

* [ ] Other
    * [ ] doc / anonmymous namespace for several functions (code ballads)
    * [ ] hana notebooks based on examples?
    * [ ] test range v3

* [ ] FPlus :
    * [ ] Officialy use it as a submodule
    * [ ] Document this and advertise
    * [ ] string_tree : delete, only lhr_rhs
    * [ ] remove bakports
    * [ ] propose extensions


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


* [X] Passer le code en east const
* [X] includes avec <>
* [X] classer fonctions string dans stringutils
* [X] cleantype or ConsType ?

* [X] Change name lib : clean type ?
    * [X] Change copyright messsage & add one empty line


* [X] Check references: (same, and for compile time)
    * [X] boost::hana::experimental::type_name
    * [X] ctti : Compile Time Type Information for C++
