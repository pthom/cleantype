
if (CLEANTYPE_COMPILETIME)
    if (
           (${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang")
        OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        OR (MSVC)
        )

        if (CLEANTYPE_USE_CONAN)
            set(Boost_INCLUDE_DIRS ${CONAN_INCLUDE_DIRS})
        else()
            find_package(Boost REQUIRED)
        endif()

        message("Boost_INCLUDE_DIRS is ${Boost_INCLUDE_DIRS}")
        function(add_hana_include target)
            target_include_directories(${target} PRIVATE ${Boost_INCLUDE_DIRS})
            message(target_include_directories(${target} PRIVATE ${Boost_INCLUDE_DIRS}))
        endfunction()

        add_executable(ct_compiler_typename ct_compiler_typename.cpp)
        clean_type_set_target_option(ct_compiler_typename)
        add_hana_include(ct_compiler_typename)
        install(TARGETS ct_compiler_typename DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

        add_executable(ct_compiler_typename_example EXCLUDE_FROM_ALL example/ct_compiler_typename_example.cpp)
        clean_type_set_target_option(ct_compiler_typename_example)
        add_hana_include(ct_compiler_typename_example)

    else()
        message(WARNING "Compile time type info only works with Clang, AppleClang and MSVC !")
    endif()

endif()
