
if (BUILD_TESTS)
  add_executable(sample_lib_test
    tests/test_main.cpp
    tests/sample_lib_test.cpp
  )
  target_include_directories(sample_lib_test PRIVATE ${doctest_INCLUDES})
  add_test(NAME sample_lib_test COMMAND sample_lib_test)
  target_link_libraries(sample_lib_test PRIVATE sample_lib)
endif()