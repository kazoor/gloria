cmake_minimum_required(VERSION 3.3)
project(sandbox)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

# add_compile_definitions(GL_API GL_IMPORT)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
get_filename_component(PARENT_DIR ../ ABSOLUTE)
set(BIN_FOLDER ${PARENT_DIR}/bin)

include_directories(${PARENT_DIR}/engine/include/)

add_executable(sandbox 
    "${SRC}/main.cpp"
)

set_target_properties(sandbox
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY   "${CMAKE_BINARY_DIR}/bin/$<0:>"
        LIBRARY_OUTPUT_DIRECTORY   "${CMAKE_BINARY_DIR}/bin/$<0:>"
        RUNTIME_OUTPUT_DIRECTORY   "${CMAKE_BINARY_DIR}/bin/$<0:>"
        PREFIX ""
        OUTPUT_NAME "sandbox"
        CXX_STANDARD 20
    )

target_link_libraries(sandbox PRIVATE engine)