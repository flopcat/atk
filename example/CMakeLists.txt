cmake_minimum_required(VERSION 3.11.3)
project(example VERSION 0.1 DESCRIPTION "AToolKitExample")

file(GLOB headers "../library/include/atk/*.h")
file(GLOB sources "src/*.cpp")
file(GLOB manifests "src/*.manifest")
file(GLOB resources "src/*.rc")
add_executable(example WIN32 ${sources} ${headers} ${manifests} ${resources})

#include_directories(".")
target_compile_definitions(example PRIVATE UNICODE _UNICODE NOMINMAX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_BUILD_TYPE RelWithDebInfo)
target_include_directories(example PRIVATE ../library/include)
target_include_directories(example PRIVATE src)
target_link_libraries(example atk kernel32 user32 gdi32)
set_target_properties(example PROPERTIES
    VERSION ${PROJECT_VERSION}
    LINKER_LANGUAGE CXX
    ARCHIVE_OUTPUT_DIRECTORY "./lib"
    LIBRARY_OUTPUT_DIRECTORY "./lib"
    RUNTIME_OUTPUT_DIRECTORY "./bin")

# Custom targets for some editors
add_custom_target(ExHeaders SOURCES ${headers})
add_custom_target(ExSources SOURCES ${sources})