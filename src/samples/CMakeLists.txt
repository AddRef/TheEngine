cmake_minimum_required(VERSION 3.0.2)

add_subdirectory(simple_window)
add_subdirectory(triangle)

set_property (TARGET simple_window PROPERTY FOLDER "samples")
set_property (TARGET triangle PROPERTY FOLDER "samples")