function(clean_type_set_target_option target)
    if (MSVC)
        target_compile_options(${target} PRIVATE /std:c++latest)
        target_compile_definitions(${target} PRIVATE
            -D_SILENCE_CXX17_IS_LITERAL_TYPE_DEPRECATION_WARNING
            -D_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING
            )
    endif()
    target_include_directories(${target} PRIVATE ${CMAKE_SOURCE_DIR}/src/include)
endfunction()


if (CLEANTYPE_COMPILETIME)

    function(clean_type_set_target_option_compile_time target)
        target_compile_definitions(${target} PRIVATE -DCLEANTYPE_COMPILETIME)
        target_include_directories(${target} PRIVATE ${boost_INCLUDE_DIRS})
    endfunction()

    find_package(Boost REQUIRED)

    add_executable(cleantype_compile_time_example example/cleantype_compile_time_example.cpp)
    clean_type_set_target_option(cleantype_compile_time_example)
    clean_type_set_target_option_compile_time(cleantype_compile_time_example)

    add_executable(cleantype_compiler_parser cleantype_compiler_parser.cpp)
    clean_type_set_target_option(cleantype_compiler_parser)
    clean_type_set_target_option_compile_time(cleantype_compiler_parser)
endif()

add_executable(cleantype_decipher_example example/cleantype_decipher_example.cpp)
clean_type_set_target_option(cleantype_decipher_example)

add_executable(cleantype_decipherer cleantype_decipherer.cpp)
clean_type_set_target_option(cleantype_decipherer)
