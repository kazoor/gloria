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
target_link_libraries(sandbox PUBLIC ${Vulkan_LIBRARIES})

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

include_directories("${PARENT_DIR}/engine/external/glfw")
target_link_libraries(sandbox PUBLIC glfw)