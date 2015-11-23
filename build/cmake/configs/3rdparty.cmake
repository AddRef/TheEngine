include (${BUILD_DIR}/cmake/utils/install.cmake)

if (WIN32)
    set (3RDPARTY_UNPACK_DIR ${ROOT_DIR}/3rdparty/_unpack/windows)
    set (BIN2CPP ${3RDPARTY_UNPACK_DIR}/bin2cpp/bin2cpp.exe)
elseif(APPLE)
    set (3RDPARTY_UNPACK_DIR ${ROOT_DIR}/3rdparty/_unpack/mac)
    set (BIN2CPP ${3RDPARTY_UNPACK_DIR}/bin2cpp/bin2cpp_mac)
else()
    set (3RDPARTY_UNPACK_DIR ${ROOT_DIR}/3rdparty/_unpack/linux)
    set (BIN2CPP ${3RDPARTY_UNPACK_DIR}/bin2cpp/bin2cpp)
endif()
set (BOOST_DIR ${3RDPARTY_UNPACK_DIR}/boost)

# Unpack 3rd party
option (ENABLE_BOOST OFF)
message("========================================================")
message("THAT COULD TAKE SOME TIME. PLEASE BE PATIENT. IT'S ONE TIME ACTION.")
# Unpack list of archives specified by configuration file
set (UNPACK_SCRIPT python ${BUILD_DIR}/script/unpack.py -i ${ROOT_DIR}/3rdparty -o ${3RDPARTY_UNPACK_DIR} -c ${BUILD_DIR}/build_config.xml)
launch_process(UNPACK_SCRIPT ${ROOT_DIR}/3rdparty/)
# Launch boost build if boost was unpacked (which happens only when boost archive has been updated)
# At the end script will copy headers and libs and will remove boost directory
set (BOOST_BUILD_SCRIPT python ${BUILD_DIR}/script/boost.py -b -r -t ${BOOST_DIR}/include -l ${BOOST_DIR}/libs -c ${BUILD_DIR}/build_config.xml)
launch_process(BOOST_BUILD_SCRIPT ${ROOT_DIR}/3rdparty/)
# Add boost dir to file cache
set (CACHE_FILE ${3RDPARTY_UNPACK_DIR}/file_cache)
set (FILE_CACHE_SCRIPT python ${BUILD_DIR}/script/file_cache.py -u ${BOOST_DIR} -c ${CACHE_FILE})
launch_process(FILE_CACHE_SCRIPT ".")
message("========================================================")

# Boost config
if (ENABLE_BOOST)
    set(Boost_USE_STATIC_LIBS       ON)
    set(Boost_USE_STATIC_RUNTIME    ON)
    set(Boost_USE_MULTITHREADED     ON)
    set(BOOST_INCLUDEDIR ${BOOST_DIR}/include/)
    set(BOOST_LIBRARYDIR ${BOOST_DIR}/libs/)
    set(Boost_DEBUG ON)
    find_package(Boost COMPONENTS log system thread log_setup REQUIRED)
endif()

# SDL config
set(SDL_STATIC OFF)
if (WIN32)
    add_definitions(-DSDL_VIDEO_OPENGL_EGL)
    add_definitions(-DSDL_VIDEO_OPENGL_ES2)
endif()

# Configure WinSDK
# if (WIN32)
#     if(${ARCH_64})
#         set (WINSDK_DIR ${3RDPARTY_DIR}/_unpack/WindowsKits/8.0/Lib/win8/um/x64)
#     else()
#         set (WINSDK_DIR ${3RDPARTY_DIR}/_unpack/WindowsKits/8.0/Lib/win8/um/x86)
#     endif()
#     set (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${WINSDK_DIR})
# endif()

# find required packages
find_package(OpenGL REQUIRED)

