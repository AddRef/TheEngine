cmake_minimum_required(VERSION 3.0.2)

file(GLOB HEADERS "*.h")
file(GLOB SOURCE "*.cpp")

add_library(resource_loader 
    ${HEADERS}
    ${SOURCE}
)

target_include_directories(resource_loader
    PUBLIC 
    .
    PRIVATE 
    ${ROOT_DIRECTORY}/utils
)

target_link_libraries(resource_loader log)