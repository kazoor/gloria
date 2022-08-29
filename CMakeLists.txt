cmake_minimum_required(VERSION 3.3)
project(gloria)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(DEBUG)
add_compile_definitions(GLORIA_PLATFORM_WINDOWS)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/engine")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/sandbox")

set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders)

set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders)
file(GLOB SHADERS ${SHADER_DIR}/*.vert ${SHADER_DIR}/*.frag ${SHADER_DIR}/*.comp ${SHADER_DIR}/*.geom ${SHADER_DIR}/*.tesc ${SHADER_DIR}/*.tese ${SHADER_DIR}/*.mesh ${SHADER_DIR}/*.task ${SHADER_DIR}/*.rgen ${SHADER_DIR}/*.rchit ${SHADER_DIR}/*.rmiss)