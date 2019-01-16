function(add_main_tests)
    file(GLOB_RECURSE cpp_files ${CMAKE_SOURCE_DIR}/src/tests/*.cpp)
    file(GLOB_RECURSE hpp_files ${CMAKE_SOURCE_DIR}/src/include/cleantype/*.hpp)

    set(files ${cpp_files} ${hpp_files})

    set(doctest_INCLUDES ${CMAKE_SOURCE_DIR}/third_party/doctest/doctest)
    set(rangev3_INCLUDES ${CMAKE_SOURCE_DIR}/third_party/tests/range-v3/include)

    add_executable(cleantype_test ${files})
    target_include_directories(cleantype_test PRIVATE ${doctest_INCLUDES} ${rangev3_INCLUDES})
    clean_type_set_target_option(cleantype_test)
    source_group(TREE ${CMAKE_SOURCE_DIR}/src/include/ FILES ${hpp_files})
    source_group(TREE ${CMAKE_SOURCE_DIR}/src/tests/ FILES ${cpp_files})

    add_test(NAME cleantype_test COMMAND cleantype_test)
endfunction()

add_main_tests()
