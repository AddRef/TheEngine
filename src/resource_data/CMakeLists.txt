cmake_minimum_required(VERSION 3.0.2)

file(GLOB HEADERS "*.h")
file(GLOB SOURCE "*.cpp")

add_library(resource_data 
    ${HEADERS}
    ${SOURCE}
)

target_include_directories(resource_data
    PUBLIC 
    .
    PRIVATE 
    ${ROOT_DIRECTORY}/utils
)

target_link_libraries(resource_data hxx_utils tinyobjloader)
set_property (TARGET resource_data PROPERTY FOLDER "core")
