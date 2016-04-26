cmake_minimum_required(VERSION 3.0.2)

file(GLOB CORE_FILES "core/*.h" "core/*.cpp")
file(GLOB RENDERER_FILES "renderer/*.h" "renderer/*.cpp")
file(GLOB RESOURCE_DATA_FILES "resource_data/*.h" "resource_data/*.cpp")

add_library(engine_core 
	${CORE_FILES}
    ${RENDERER_FILES}
    ${RESOURCE_DATA_FILES}
)

target_include_directories(engine_core
	PUBLIC
	.
	PRIVATE 
	.
	${ROOT_DIRECTORY}/utils
)

target_link_libraries(engine_core hxx_utils SDL2 tinyobjloader)
set_property (TARGET engine_core PROPERTY FOLDER "engine_core")

source_group("core" FILES ${CORE_FILES})
source_group("renderer" FILES ${RENDERER_FILES})
source_group("resource_data" FILES ${RESOURCE_DATA_FILES})
