cmake_minimum_required (VERSION 3.8.1)

project(cleantype)

option(CLEANTYPE_BUILD_TESTS "Build tests." ON)
option(CLEANTYPE_COMPILETIME "Compile time type info (require Boost.Hana)." OFF)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  message("ccache found -> using it")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

add_definitions("-std=c++14")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})

# Enable tests via cmake_register_test and doctest
set(doctest_INCLUDES ${CMAKE_SOURCE_DIR}/third_party/doctest/doctest)
enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/third_party/FunctionalPlus/include)
include_directories(${CMAKE_SOURCE_DIR}/src/include)

if (CLEANTYPE_BUILD_TESTS)
  add_subdirectory(src/include/cleantype/tests)
endif()

add_subdirectory(src/include/cleantype/compile_time)
