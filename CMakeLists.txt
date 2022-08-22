cmake_minimum_required(VERSION 3.3)
project(gloria)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(gloria 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/glm/")

set_target_properties( gloria
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        PREFIX ""
        OUTPUT_NAME "gloria"
        CXX_STANDARD 20
        )

message(STATUS "Compiling GLFW")
# Build settings for GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add GLFW subdirectory and link it
message(STATUS "Compiling & Linking GLFW")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glfw")
target_link_libraries(gloria PUBLIC glfw)