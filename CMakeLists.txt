cmake_minimum_required (VERSION 3.8.1)

project(cleantype)

option(CLEANTYPE_BUILD_TESTS "Build tests." ON)
option(CLEANTYPE_USE_CONAN "Find deps (Boost) using conan." OFF)
option(CLEANTYPE_BUILD_TOOLS "Build tools (ct_compiler_decipher)." OFF)

option(CLEANTYPE_ALL_OPTIONS "Use conan / build tests / build compile time" OFF)

if (CLEANTYPE_ALL_OPTIONS)
  set(CLEANTYPE_BUILD_TESTS ON)
  set(CLEANTYPE_USE_CONAN ON)
  set(CLEANTYPE_BUILD_TOOLS ON)
endif()

enable_testing()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (CLEANTYPE_USE_CONAN)
  # execute_process(
  #   COMMAND conan install ${CMAKE_SOURCE_DIR} --build=missing ${conan_compiler_option}
  #   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  #   RESULT_VARIABLE conan_result
  # )
  # if(NOT "${conan_result}" STREQUAL "0")
  #       message(FATAL_ERROR "Fatal error when running 'conan install ${CMAKE_SOURCE_DIR}'")
  # endif()
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

function(clean_type_set_target_option target)
    if (MSVC)
        target_compile_options(${target} PRIVATE /std:c++latest)
        target_compile_definitions(${target} PRIVATE
            -D_SILENCE_CXX17_IS_LITERAL_TYPE_DEPRECATION_WARNING
            -D_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING
            )
    endif()
    target_include_directories(${target} PRIVATE ${CMAKE_SOURCE_DIR}/src/include)
    target_include_directories(${target} PRIVATE ${CMAKE_SOURCE_DIR}/third_party/FunctionalPlus/include)
    if (APPLE)
      target_link_libraries(${target} PRIVATE c++)
    endif()
    if (UNIX)
      target_link_libraries(${target} PRIVATE stdc++ m)
    endif()
endfunction()

if (CLEANTYPE_BUILD_TESTS)
  add_subdirectory(src/include/cleantype/tests)
endif()

if (CLEANTYPE_BUILD_TOOLS)
  add_subdirectory(src/tools/ct_compiler_decipher)
endif()
