
file(GLOB_RECURSE cpp_files ${CMAKE_SOURCE_DIR}/src/include/cleantype/tests/*.cpp)
file(GLOB_RECURSE hpp_files ${CMAKE_SOURCE_DIR}/src/include/cleantype/*.hpp)

set(files ${cpp_files} ${hpp_files})

add_executable(cleantype_test ${files})
add_test(NAME cleantype_test COMMAND cleantype_test)
message("doctest_INCLUDES=${doctest_INCLUDES}")
target_include_directories(cleantype_test PRIVATE ${doctest_INCLUDES})
target_link_libraries(cleantype_test PRIVATE CONAN_PKG::boost)

if (MSVC)
    target_compile_options(cleantype_test PRIVATE /std:c++latest)
    target_compile_definitions(cleantype_test PRIVATE
        -D_SILENCE_CXX17_IS_LITERAL_TYPE_DEPRECATION_WARNING
        -D_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING
        )
endif()

add_executable(type_name_stringliteral_test ../details/hana_ext/experimental/detail/test/type_name_stringliteral_test.cpp)

source_group(TREE ${CMAKE_SOURCE_DIR}/src/include/ FILES ${files})
