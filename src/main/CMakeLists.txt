add_executable(main main.cpp)
# target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(main PRIVATE sample_lib)
