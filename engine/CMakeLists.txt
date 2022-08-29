cmake_minimum_required(VERSION 3.7)
project(engine)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# add_compile_definitions(GL_API GL_EXPORT)

add_library(engine STATIC
    "${SRC}/core/application/application.cpp"
    "${SRC}/core/instance/instance.cpp"
    "${SRC}/core/events/eventhandler/eventhandler.cpp"
    "${SRC}/core/layers/layerstack/layerstack.cpp"
    "${SRC}/core/window/window.cpp"
    "${SRC}/utils/log/log.cpp"
    "${SRC}/utils/input/inputhandler/inputhandler.cpp"
    "${SRC}/defines.hpp"
    "${SRC}/vulkan/vulkaninstance/vulkaninstance.cpp"
)

set_target_properties(engine
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY   "${CMAKE_BINARY_DIR}/bin/$<0:>"
        LIBRARY_OUTPUT_DIRECTORY   "${CMAKE_BINARY_DIR}/bin/$<0:>"
        RUNTIME_OUTPUT_DIRECTORY   "${CMAKE_BINARY_DIR}/bin/$<0:>"
        PREFIX ""
        OUTPUT_NAME "engine"
        CXX_STANDARD 20
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

message(STATUS "Compiling GLFW")
# Build settings for GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add GLFW subdirectory and link it
message(STATUS "Compiling & Linking GLFW")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glfw")
target_link_libraries(engine PUBLIC glfw)

target_link_libraries(engine PUBLIC ${Vulkan_LIBRARIES})

message(STATUS "Compiling & Linking SPDLOG")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/spdlog")
target_link_libraries(engine PUBLIC spdlog)

target_include_directories(engine SYSTEM PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/external")