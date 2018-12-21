if (CLEANTYPE_COMPILETIME)

    find_package(Boost REQUIRED)

    function(add_hana_include target)
        target_include_directories(${target} PRIVATE ${boost_INCLUDE_DIRS})
    endfunction()


    add_executable(ct_compiler_typeinfo ct_compiler_typeinfo.cpp)
    clean_type_set_target_option(ct_compiler_typeinfo)
    add_hana_include(ct_compiler_typeinfo)

    add_executable(ct_compiler_typeinfo_example example/ct_compiler_typeinfo_example.cpp)
    clean_type_set_target_option(ct_compiler_typeinfo_example)
    add_hana_include(ct_compiler_typeinfo_example)

endif()
