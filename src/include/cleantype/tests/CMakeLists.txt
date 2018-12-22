function(add_main_tests)
    file(GLOB_RECURSE cpp_files ${CMAKE_SOURCE_DIR}/src/include/cleantype/tests/*.cpp)
    file(GLOB_RECURSE hpp_files ${CMAKE_SOURCE_DIR}/src/include/cleantype/*.hpp)

    set(files ${cpp_files} ${hpp_files})

    set(doctest_INCLUDES ${CMAKE_SOURCE_DIR}/third_party/doctest/doctest)

    add_executable(cleantype_test ${files})
    target_include_directories(cleantype_test PRIVATE ${doctest_INCLUDES})
    clean_type_set_target_option(cleantype_test)
    source_group(TREE ${CMAKE_SOURCE_DIR}/src/include/ FILES ${files})

    add_test(NAME cleantype_test COMMAND cleantype_test)
endfunction()

add_main_tests()
add_executable(type_name_stringliteral_test ../details/hana_type_name/test/type_name_stringliteral_test.cpp)
clean_type_set_target_option(type_name_stringliteral_test)
