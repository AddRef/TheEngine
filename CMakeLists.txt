cmake_minimum_required(VERSION 3.0.2)
set_property (GLOBAL PROPERTY USE_FOLDERS ON)
project(TheGame)

set (ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set (BUILD_DIR ${ROOT_DIR}/build)
set (SRC_DIRECTORY ${ROOT_DIR}/src)

include(${BUILD_DIR}/cmake/configs/global.cmake)
include(${BUILD_DIR}/cmake/configs/3rdparty.cmake)
include(${BUILD_DIR}/cmake/utils/compiler.cmake)
include(${BUILD_DIR}/cmake/utils/platform.cmake)

update_compiler_settings()
check_apple_platform()

# Add projects
add_subdirectory(3rdparty)
add_subdirectory(src)