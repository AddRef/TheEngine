cmake_minimum_required(VERSION 3.0.2)

disable_warning_level()

add_subdirectory(${3RDPARTY_UNPACK_DIR}/gtest)
add_subdirectory(${3RDPARTY_UNPACK_DIR}/lua)
add_subdirectory(${3RDPARTY_UNPACK_DIR}/SDL2)
add_subdirectory(${3RDPARTY_UNPACK_DIR}/tinyobjloader)

set_property (TARGET gtest            PROPERTY FOLDER "3rdparty")
set_property (TARGET lua              PROPERTY FOLDER "3rdparty")
set_property (TARGET SDL2             PROPERTY FOLDER "3rdparty")
set_property (TARGET SDL2main         PROPERTY FOLDER "3rdparty")
set_property (TARGET uninstall        PROPERTY FOLDER "3rdparty")
set_property (TARGET tinyobjloader    PROPERTY FOLDER "3rdparty")
if (SDL_STATIC)
    set_property (TARGET SDL2-static      PROPERTY FOLDER "3rdparty/SDL")
	target_include_directories(SDL2-static PUBLIC ${3RDPARTY_UNPACK_DIR}/SDL2/include)
endif()

target_compile_definitions(gtest PUBLIC GTEST_HAS_TR1_TUPLE=0 GTEST_USE_OWN_TR1_TUPLE=0)
target_include_directories(SDL2 PUBLIC ${3RDPARTY_UNPACK_DIR}/SDL2/include)
target_include_directories(tinyobjloader PUBLIC ${3RDPARTY_UNPACK_DIR}/tinyobjloader)

