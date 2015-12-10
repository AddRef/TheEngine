cmake_minimum_required(VERSION 3.0.2)

file(GLOB HEADERS "*.h")
file(GLOB SOURCE "*.cpp")

add_library(math STATIC 
    ${HEADERS}
    ${SOURCE}
)
target_include_directories(math
    PUBLIC
    .
    ${3RDPARTY_UNPACK_DIR}/glm
)