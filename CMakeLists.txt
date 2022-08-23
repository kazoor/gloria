cmake_minimum_required(VERSION 3.3)
project(gloria)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(DEBUG)

add_executable(gloria 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/application/application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/logger/logger.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/window/window.hpp"
)

if (DEFINED VULKAN_SDK_PATH)
    set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include") # 1.1 Make sure this include path is correct
    set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib") # 1.2 Make sure lib path is correct
    set(Vulkan_FOUND "True")
else()
    find_package(Vulkan REQUIRED)
    message(STATUS "Found vulkan: $ENV{VULKAN_SDK}")
endif()
if (NOT Vulkan_FOUND)
    message(STATUS "Couldn't find Vulkan D:")
else()
    message(STATUS "Using vulkan lib at ${Vulkan_LIBRARIES}")
endif()

include_directories("${Vulkan_INCLUDE_DIRS}")
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

target_link_libraries(gloria PUBLIC ${Vulkan_LIBRARIES})

# Add spldog subdirectory and link it
message(STATUS "Compiling & Linking SPDLOG")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/spdlog")
target_link_libraries(gloria PUBLIC spdlog)

target_include_directories(gloria SYSTEM PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/external")