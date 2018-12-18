
file(GLOB_RECURSE cpp_files ${CMAKE_SOURCE_DIR}/src/include/type_name/tests/*.cpp)
file(GLOB_RECURSE hpp_files ${CMAKE_SOURCE_DIR}/src/include/type_name/*.hpp)

set(files ${cpp_files} ${hpp_files})

add_executable(typename_test ${files})
add_test(NAME typename_test COMMAND typename_test)
message("doctest_INCLUDES=${doctest_INCLUDES}")
target_include_directories(typename_test PRIVATE ${doctest_INCLUDES})
target_link_libraries(typename_test PRIVATE CONAN_PKG::boost)

if (MSVC)
    target_compile_options(typename_test PRIVATE /std:c++latest)
    target_compile_definitions(typename_test PRIVATE
        -D_SILENCE_CXX17_IS_LITERAL_TYPE_DEPRECATION_WARNING
        -D_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING
        )
endif()

add_executable(type_name_cstring_test ../details/hana_ext/experimental/detail/test/type_name_cstring_test.cpp)

source_group(TREE ${CMAKE_SOURCE_DIR}/src/include/ FILES ${files})
