cmake_minimum_required(VERSION 3.8)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

project(Main LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(main)
add_subdirectory("src")

target_include_directories(main PUBLIC "third_party/cxxopts/include")
target_precompile_headers(main PUBLIC "pch.hpp")

set(DEBUG_COMPILE_FLAGS "-Wall" "-Wextra" "-Wpedantic")
set(DEBUG_LINK_FLAGS "-Wall")

target_compile_options(main PUBLIC ${DEBUG_COMPILE_FLAGS}) 
target_link_options(main PUBLIC ${DEBUG_LINK_FLAGS})
