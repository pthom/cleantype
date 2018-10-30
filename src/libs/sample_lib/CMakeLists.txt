add_library(sample_lib_object OBJECT sample_lib.cpp)
crt_registertest(
  TEST_INPLACE
  INPUT_OBJECT_LIBRARY sample_lib_object
  OUTPUT_LIBRARY_NAME sample_lib
  OUTPUT_LIBRARY_TYPE STATIC
  OUTPUT_TEST_TARGET sample_lib_test
)