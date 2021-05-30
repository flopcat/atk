cmake_minimum_required(VERSION 3.8)
project(atk)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(library)
add_subdirectory(example)

set_target_properties(atk PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../release/")
set_target_properties(example PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../release/")
