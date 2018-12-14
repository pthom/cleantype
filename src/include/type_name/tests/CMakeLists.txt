
file(GLOB_RECURSE cpp_files ${CMAKE_SOURCE_DIR}/src/include/type_name/tests/*.cpp)
file(GLOB_RECURSE hpp_files ${CMAKE_SOURCE_DIR}/src/include/type_name/*.hpp)

set(files ${cpp_files} ${hpp_files})

add_executable(typename_test ${files})
add_test(NAME typename_test COMMAND typename_test)
message("doctest_INCLUDES=${doctest_INCLUDES}")
target_include_directories(typename_test PRIVATE ${doctest_INCLUDES})
target_link_libraries(typename_test PRIVATE CONAN_PKG::boost)
source_group(TREE ${CMAKE_SOURCE_DIR}/src/include/ FILES ${files})
