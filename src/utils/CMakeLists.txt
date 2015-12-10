cmake_minimum_required(VERSION 3.0.2)

add_subdirectory(log)
add_subdirectory(math)
set_property (TARGET log PROPERTY FOLDER "utils")
