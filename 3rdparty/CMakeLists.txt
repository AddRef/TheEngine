cmake_minimum_required(VERSION 3.0.2)

disable_warning_level()

add_subdirectory(_unpack/gtest)
add_subdirectory(_unpack/lua)
add_subdirectory(_unpack/SDL2)

set_property (TARGET gtest            PROPERTY FOLDER "3rdparty/gtest")
set_property (TARGET lua              PROPERTY FOLDER "3rdparty/lua")
set_property (TARGET SDL2             PROPERTY FOLDER "3rdparty/SDL")
set_property (TARGET SDL2main         PROPERTY FOLDER "3rdparty/SDL")
if (SDL_STATIC)
    set_property (TARGET SDL2-static      PROPERTY FOLDER "3rdparty/SDL")
endif()

set_target_properties(gtest PROPERTIES COMPILE_FLAGS -DGTEST_HAS_TR1_TUPLE=0)
