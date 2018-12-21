cmake_minimum_required (VERSION 3.8.1)

project(cleantype)

option(CLEANTYPE_BUILD_TESTS "Build tests." ON)
option(CLEANTYPE_COMPILETIME "Compile time type info (require Boost.Hana)." OFF)
option(CLEANTYPE_USE_CONAN "Find deps (Boost) using conan." OFF)

option(CLEANTYPE_ALL_OPTIONS "Use conan / build tests / build compile time" OFF)

if (CLEANTYPE_ALL_OPTIONS)
  set(CLEANTYPE_BUILD_TESTS ON)
  set(CLEANTYPE_COMPILETIME ON)
  set(CLEANTYPE_USE_CONAN ON)
endif()


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (CLEANTYPE_USE_CONAN)
  execute_process(
    COMMAND conan install ${CMAKE_SOURCE_DIR} --build=missing
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    RESULT_VARIABLE conan_result
  )
  if(NOT "${conan_result}" STREQUAL "0")
        message(FATAL_ERROR "Fatal error when running 'conan install ${CMAKE_SOURCE_DIR}'")
  endif()

  include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
  conan_basic_setup(TARGETS)
endif()


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
