cmake_minimum_required(VERSION 3.0.2)

file(GLOB HEADERS "*.h")
file(GLOB SOURCE "*.cpp")

add_executable(triangle 
    ${HEADERS}
    ${SOURCE}
)

target_link_libraries(triangle engine_core resource_loader math)