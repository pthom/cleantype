add_library(sample_lib sample_lib.cpp)

# Below : example in order to link with a conan library
# target_link_libraries(sample_lib CONAN_PKG::opencv)

if (BUILD_TESTS)
  crt_registertest(
    LIBRARY sample_lib
    OUTPUT_LIBRARY_NAME sample_lib
    OUTPUT_TEST_TARGET sample_lib_test
    TEST_SOURCES tests/sample_lib_test.cpp
  )
endif()