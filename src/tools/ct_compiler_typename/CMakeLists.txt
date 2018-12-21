if (CLEANTYPE_COMPILETIME)

    find_package(Boost REQUIRED)

    function(add_hana_include target)
        target_include_directories(${target} PRIVATE ${boost_INCLUDE_DIRS})
    endfunction()


    add_executable(ct_compiler_typename ct_compiler_typename.cpp)
    clean_type_set_target_option(ct_compiler_typename)
    add_hana_include(ct_compiler_typename)
    install(TARGETS ct_compiler_typename DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

    add_executable(ct_compiler_typename_example EXCLUDE_FROM_ALL example/ct_compiler_typename_example.cpp)
    clean_type_set_target_option(ct_compiler_typename_example)
    add_hana_include(ct_compiler_typename_example)

endif()
