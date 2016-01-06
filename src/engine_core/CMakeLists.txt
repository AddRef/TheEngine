cmake_minimum_required(VERSION 3.0.2)

file(GLOB PUBLIC_HEADERS "public/*.h")
file(GLOB HEADERS "*.h")
file(GLOB SOURCE "*.cpp")

add_library(engine_core 
	${PUBLIC_HEADERS}
    ${HEADERS}
	${SOURCE}
)

target_include_directories(engine_core
	PUBLIC 
	./public
	PRIVATE 
	.
	${ROOT_DIRECTORY}/utils
)

target_link_libraries(engine_core log SDL2 resource_data)
set_property (TARGET engine_core PROPERTY FOLDER "core")
