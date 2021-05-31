cmake_minimum_required(VERSION 3.11.3)
project(atk VERSION 0.1 DESCRIPTION "AToolKit")

file(GLOB headers "include/atk/*.h")
file(GLOB sources "src/*.cpp")
add_library(atk SHARED ${sources} ${headers})

#include_directories(".")
target_compile_definitions(atk PRIVATE UNICODE _UNICODE NOMINMAX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_SHARED_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_BUILD_TYPE RelWithDebInfo)
target_include_directories(atk PRIVATE include)
target_include_directories(atk PRIVATE src)
target_link_libraries(atk kernel32 user32 gdi32 comctl32)
set_target_properties(atk PROPERTIES
    VERSION ${PROJECT_VERSION}
    LINKER_LANGUAGE CXX
    ARCHIVE_OUTPUT_DIRECTORY "./lib"
    LIBRARY_OUTPUT_DIRECTORY "./lib"
    RUNTIME_OUTPUT_DIRECTORY "./lib")

# Custom targets for some editors
add_custom_target(Headers SOURCES ${headers})
add_custom_target(Sources SOURCES ${sources})