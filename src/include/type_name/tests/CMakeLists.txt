
file(GLOB_RECURSE cpp_files ${CMAKE_SOURCE_DIR}/src/include/type_name/tests/*.cpp)
file(GLOB_RECURSE hpp_files ${CMAKE_SOURCE_DIR}/src/include/type_name/*.hpp)

set(files ${cpp_files} ${hpp_files})

add_executable(typename_test ${files})
add_test(NAME typename_test COMMAND typename_test)    
target_include_directories(typename_test PRIVATE ${doctest_INCLUDES})
source_group(TREE ${CMAKE_SOURCE_DIR}/src/include/ FILES ${files})
