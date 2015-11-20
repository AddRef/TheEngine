cmake_minimum_required(VERSION 3.0.2)

file(GLOB PUBLIC_HEADERS "include/*.h")
file(GLOB PRIVATE_HEADERS "src/*.h")
file(GLOB SOURCE "src/*.cpp")

add_library(engine_core SHARED 
	${PUBLIC_HEADERS}
    ${PRIVATE_HEADERS}
	${SOURCE}
)

target_include_directories(engine_core
	PUBLIC 
	./include
	PRIVATE 
	${ROOT_DIRECTORY}/utils
)

target_link_libraries(engine_core log SDL2)