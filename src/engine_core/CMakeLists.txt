cmake_minimum_required(VERSION 3.0.2)

file(GLOB HEADERS "*.h")
file(GLOB SOURCE "*.cpp")

add_library(engine_core 
    ${HEADERS}
	${SOURCE}
)

target_include_directories(engine_core
	PUBLIC 
	.
	PRIVATE 
	${ROOT_DIRECTORY}/utils
)

target_link_libraries(engine_core log SDL2)