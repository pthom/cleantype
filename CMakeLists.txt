cmake_minimum_required (VERSION 3.8.1)

project(snake_cpp)

add_definitions("-std=c++14")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set (crt_test_lib_location ${CMAKE_SOURCE_DIR}/third_party/doctest/doctest)
include("${CMAKE_SOURCE_DIR}/cmake/cmake_registertest/src/crt_registertest_doctest.cmake")
enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/third_party/FunctionalPlus/include)
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

add_subdirectory(src/libs/sample_lib)
add_subdirectory(src/main)
