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

if (TODO_USE_CLANG_TIDY)
    find_program(CLANG_TIDY "clang-tidy")
    if (CLANG_TIDY)
        set(CMAKE_CXX_CLANG_TIDY  ${CLANG_TIDY})
    else()
        message(SEND_ERROR "clang-tidy requested but executable not found")
    endif()
endif()

if(WIN32)
    add_definitions(-D_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options ("-fdiagnostics-color=always")
    add_compile_options("-Wall" "-Wextra" "-Wpedantic")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options ("-fcolor-diagnostics")
    add_compile_options("-Wall" "-Wextra" "-Wpedantic")
endif ()

set(SPDLOG_FMT_EXTERNAL ON)

find_package(cereal)
find_package(cxxopts)
find_package(date)
find_package(doctest)
find_package(spdlog)
find_package(fmt)
find_package(magic_enum)

add_library(todolib)
target_include_directories(todolib PUBLIC "src/todo")

if (TODO_USE_PCH)
    target_precompile_headers(todolib PUBLIC "pch.hpp")
endif()

add_subdirectory("src/todo")

target_link_libraries(todolib PUBLIC cereal::cereal cxxopts::cxxopts date::date doctest::doctest spdlog::spdlog fmt::fmt magic_enum::magic_enum)

add_executable(todo "src/main.cpp")
target_link_libraries(todo PRIVATE todolib)

if (TODO_BUILD_TESTS)
    add_executable(todotest)
    add_subdirectory("test")
    target_link_libraries(todotest PRIVATE todolib)
endif()
