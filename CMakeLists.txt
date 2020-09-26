cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_COMPILER "clang++")

project(Todo LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_options("-Wall" "-Wextra" "-Wpedantic")

add_executable(todo)
add_subdirectory("src")
target_include_directories(todo PUBLIC "third_party/cxxopts/include")
target_include_directories(todo PUBLIC "third_party/spdlog/include")
target_include_directories(todo PUBLIC "third_party/fmt/include")
#target_precompile_headers(todo PUBLIC "pch.hpp")


set(SPDLOG_FMT_EXTERNAL ON)
add_subdirectory("third_party/fmt")
add_subdirectory("third_party/spdlog")

target_link_libraries(todo PRIVATE spdlog::spdlog)
target_link_libraries(todo PRIVATE fmt::fmt)
