cmake_minimum_required(VERSION 3.15)

project(Todo LANGUAGES CXX)
option(TODO_BUILD_TESTS "Build tests" ON)
option(TODO_USE_PCH "Use PCH" OFF)
option(TODO_USE_CLANG_TIDY "Use clang tidy" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_options("-Wall" "-Wextra" "-Wpedantic")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options ("-fdiagnostics-color=always" "-fconcepts")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options ("-fcolor-diagnostics")
endif ()

set(SPDLOG_FMT_EXTERNAL ON)
add_subdirectory("third_party/fmt")
add_subdirectory("third_party/spdlog")

add_library(todolib)
target_include_directories(todolib PUBLIC "third_party/cxxopts/include")
target_include_directories(todolib PUBLIC "third_party/spdlog/include")
target_include_directories(todolib PUBLIC "third_party/fmt/include")
target_include_directories(todolib PUBLIC "third_party/cereal/include")
target_include_directories(todolib PUBLIC "third_party/date/include")
target_include_directories(todolib PUBLIC "src/todo")

if (TODO_USE_PCH)
    target_precompile_headers(todolib PUBLIC "pch.hpp")
endif()

add_subdirectory("src/todo")

target_link_libraries(todolib PUBLIC spdlog::spdlog)
target_link_libraries(todolib PUBLIC fmt::fmt)

add_executable(todo "src/main.cpp")
target_link_libraries(todo PRIVATE todolib)

if (TODO_BUILD_TESTS)
    add_executable(todotest)
    add_subdirectory("test")
    target_link_libraries(todotest PRIVATE todolib)
    target_include_directories(todotest PRIVATE "third_party/doctest/doctest")
endif()

if (TODO_USE_CLANG_TIDY)
    find_program(CLANG_TIDY "clang-tidy")
    if (CLANG_TIDY)
        set_target_properties(todolib PROPERTIES CXX_CLANG_TIDY ${CLANG_TIDY})
    else()
        message(SEND_ERROR "clang-tidy requested but executable not found")
    endif()
endif()
