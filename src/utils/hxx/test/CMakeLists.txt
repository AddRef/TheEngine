cmake_minimum_required(VERSION 3.0.2)

file(GLOB HEADERS "*.h")
file(GLOB SOURCE "*.cpp")

add_executable(callbacks_test 
    ${HEADERS}
    ${SOURCE}
)

target_link_libraries(callbacks_test hxx_utils gtest)
target_include_directories(callbacks_test PRIVATE ${3RDPARTY_UNPACK_DIR}/gtest/include)
set_property (TARGET callbacks_test PROPERTY FOLDER "tests")
