cmake_minimum_required(VERSION 3.0.2)

file(GLOB HEADERS "*.h")
file(GLOB SOURCE "*.cpp")

add_executable(simple_window 
    ${HEADERS}
	${SOURCE}
)

target_link_libraries(simple_window engine_core)