cmake_minimum_required (VERSION 3.8.1)

project(cpp_skeleton)

option(BUILD_TESTS "Build tests." ON)
option(BUILD_SHARED_LIBS "Build shared instead of static libraries." ON)

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

# add cmake-init utilities (unused for the moment)
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/cmake_init)
# include(Findclang_tidy)
# include(ClangTidy)

# Enable tests via cmake_register_test and doctest
set(doctest_INCLUDES ${CMAKE_SOURCE_DIR}/third_party/doctest/doctest)
enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/third_party/FunctionalPlus/include)
include_directories(${CMAKE_SOURCE_DIR}/src/include)

add_subdirectory(src/include/type_name/tests)
