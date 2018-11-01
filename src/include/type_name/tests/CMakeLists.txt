add_executable(type_name_test type_name_test.cpp)
target_include_directories(type_name_test PRIVATE ${doctest_INCLUDES})
add_test(NAME type_name_test COMMAND type_name_test)
