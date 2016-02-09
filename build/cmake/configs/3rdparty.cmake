include (${BUILD_DIR}/cmake/utils/install.cmake)
include (${BUILD_DIR}/cmake/configs/global.cmake)

option (ENABLE_BOOST "Use Boost" ON)

set (3RDPARTY_DIR ${ROOT_DIR}/3rdparty)
if (WIN32)
    set (3RDPARTY_UNPACK_DIR ${3RDPARTY_DIR}/_unpack/windows)
    set (BIN2CPP ${3RDPARTY_UNPACK_DIR}/bin2cpp/bin2cpp.exe)
elseif(APPLE)
    set (3RDPARTY_UNPACK_DIR ${3RDPARTY_DIR}/_unpack/mac)
    set (BIN2CPP ${3RDPARTY_UNPACK_DIR}/bin2cpp/bin2cpp_mac)
else()
    set (3RDPARTY_UNPACK_DIR ${3RDPARTY_DIR}/_unpack/linux)
    set (BIN2CPP ${3RDPARTY_UNPACK_DIR}/bin2cpp/bin2cpp)
endif()
set (BOOST_DIR ${3RDPARTY_UNPACK_DIR}/boost)

# Unpack 3rd party
if (NOT WIN32 OR ${MSVC_VERSION} EQUAL 1800)
    set (BOOST_ADDITIONAL_BUILD_ARGUMENTS -a toolset=msvc-12.0) # support VS2013 for now only on Windows
endif()

file (GLOB 3RDPARTY_ARCHIVES ${3RPARTY_DIR}/*.zip ${3RPARTY_DIR}/*.bz2)
set (UNPACK_SCRIPT python ${BUILD_DIR}/script/unpack.py -i ${ROOT_DIR}/3rdparty -o ${3RDPARTY_UNPACK_DIR} -c ${BUILD_DIR}/build_config.xml)
set (BOOST_BUILD_SCRIPT python ${BUILD_DIR}/script/boost.py -b -t ${BOOST_DIR}/include -l ${BOOST_DIR}/libs -c ${BUILD_DIR}/build_config.xml -r ${BOOST_ADDITIONAL_BUILD_ARGUMENTS})
set (FILE_CACHE_SCRIPT python ${BUILD_DIR}/script/file_cache.py -u ${BOOST_DIR} -c ${3RDPARTY_UNPACK_DIR}/file_cache)
# Launch once
if (NOT FIRST_START_PROCESSED)
    launch_process(UNPACK_SCRIPT ${ROOT_DIR}/3rdparty/)
    launch_process(BOOST_BUILD_SCRIPT ${ROOT_DIR}/3rdparty/)
    launch_process(FILE_CACHE_SCRIPT ${ROOT_DIR}/3rdparty/)
    set (FIRST_START_PROCESSED TRUE CACHE BOOL "First time launch" FORCE)
endif()

# Unpack 3rdparty archives
# Launch boost build if boost was unpacked (which happens only when boost archive has been updated)
# At the end script will copy headers and libs and will remove boost directory
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/output.txt
                   COMMAND ${UNPACK_SCRIPT}
                   COMMAND ${BOOST_BUILD_SCRIPT}
                   COMMAND ${FILE_CACHE_SCRIPT}
                   COMMAND ECHO generated > ${CMAKE_CURRENT_BINARY_DIR}/output.txt 
                   WORKING_DIRECTORY ${3RPARTY_DIR}
                   DEPENDS 
                        ${3RDPARTY_ARCHIVES} 
                        ${BUILD_DIR}/build_config.xml
                        ${BUILD_DIR}/script/unpack.py
                        ${BUILD_DIR}/script/boost.py
                        ${BUILD_DIR}/script/file_cache.py
                   COMMENT "Preparing 3rdparty..."
)

add_custom_target(prepare_3rdparty ALL
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/output.txt
    SOURCES 
    ${BUILD_DIR}/build_config.xml
    ${BUILD_DIR}/script/unpack.py
    ${BUILD_DIR}/script/boost.py
    ${BUILD_DIR}/script/file_cache.py
)

set_property (TARGET prepare_3rdparty PROPERTY FOLDER "build")

# Boost config
if (ENABLE_BOOST)
    set(Boost_USE_STATIC_LIBS       ON)
    set(Boost_USE_STATIC_RUNTIME    ON)
    set(Boost_USE_MULTITHREADED     ON)
    set(BOOST_INCLUDEDIR ${BOOST_DIR}/include/)
    set(BOOST_LIBRARYDIR ${BOOST_DIR}/libs/)
    set(Boost_DEBUG OFF)
    find_package(Boost COMPONENTS log system thread log_setup REQUIRED)
endif()

# MS SDL Config
if (WIN32)
    if(${ARCH_64})
        set (ENV{DXSDK_DIR} ${3RDPARTY_UNPACK_DIR}/WindowsKits/Lib/win8/um/x64)
    else()
        set (ENV{DXSDK_DIR} ${3RDPARTY_UNPACK_DIR}/WindowsKits/Lib/win8/um/x86)
    endif()
endif()


# SDL config
set(SDL_STATIC OFF)
if (WIN32)
    add_definitions(-DSDL_VIDEO_OPENGL_EGL)
    add_definitions(-DSDL_VIDEO_OPENGL_ES2)
endif()

# find required packages
find_package(OpenGL REQUIRED)

