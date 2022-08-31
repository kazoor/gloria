cmake_minimum_required(VERSION 3.3)
project(gloria)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(DEBUG)
# add_compile_definitions(VK_USE_VERBOSE)

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
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/resources/shaders/)

file(COPY ${SHADER_DIR}/${FILENAME}.spv
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/resources/shaders/)
endForeach()

add_custom_target(shaders ALL DEPENDS ${SPV_SHADERS})

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/engine")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/sandbox")