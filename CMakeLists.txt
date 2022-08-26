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
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/vulkan/validationlayer/validationlayer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/vulkan/physicaldevice/physicaldevice.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/vulkan/logicaldevice/logicaldevice.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/vulkan/surface/windowsurface.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/vulkan/queuefamilyindices/queuefamilyindices.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/vulkan/graphicspipeline/graphicspipeline.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/vulkan/swapchain/swapchain.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/vulkan/swapchain/swapchainsupport.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/vulkan/commandbuffers/commandbuffers.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/vulkan/vulkanrenderer/vulkanrenderer.cpp"
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

set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders)
file(GLOB SHADERS ${SHADER_DIR}/*.vert ${SHADER_DIR}/*.frag ${SHADER_DIR}/*.comp ${SHADER_DIR}/*.geom ${SHADER_DIR}/*.tesc ${SHADER_DIR}/*.tese ${SHADER_DIR}/*.mesh ${SHADER_DIR}/*.task ${SHADER_DIR}/*.rgen ${SHADER_DIR}/*.rchit ${SHADER_DIR}/*.rmiss)

foreach(SHADER IN LISTS SHADERS)
    get_filename_component(FILENAME ${SHADER} NAME)
    add_custom_command(OUTPUT ${SHADER_DIR}/${FILENAME}.spv
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${SHADER} -o ${SHADER_DIR}/${FILENAME}.spv
        DEPENDS ${SHADER}
        COMMENT "Compiling ${FILENAME}")
list(APPEND SPV_SHADERS ${SHADER_DIR}/${FILENAME}.spv)

file(COPY ${SHADER_DIR}/${FILENAME}.spv
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/Release/resources/shaders/)

file(COPY ${SHADER_DIR}/${FILENAME}.spv
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/Debug/resources/shaders/)

file(COPY ${SHADER_DIR}/${FILENAME}.spv
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
endForeach()

add_custom_target(shaders ALL DEPENDS ${SPV_SHADERS})

target_include_directories(gloria SYSTEM PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/external")
