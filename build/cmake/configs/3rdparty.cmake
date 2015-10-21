# Unpack 3rd party
option (ENABLE_BOOST OFF)
message("Unpacking 3rdparty")
message("========================================================")
set (UNPACK_SCRIPT python ${BUILD_DIR}/script/unpack.py -i ${ROOT_DIR}/3rdparty -o ${3RDPARTY_UNPACK_DIR} -c ${BUILD_DIR}/script/unpack_config.xml)
message("${UNPACK_SCRIPT}")
execute_process(COMMAND ${UNPACK_SCRIPT}
				RESULT_VARIABLE var 
				ERROR_VARIABLE err
				WORKING_DIRECTORY ${ROOT_DIR}/3rdparty/)
if (var) 
	message(${var})
endif()
if (err)
	message(FATAL ${err})
endif()
message("========================================================")

if (WIN32)
    set (BOOST_DIR ${3RDPARTY_DIR}/boost_windows)
    set (BIN2CPP ${3RDPARTY_DIR}/bin2cpp/bin2cpp.exe)
elseif(APPLE)
    set (BOOST_DIR ${3RDPARTY_DIR}/boost_mac)
    set (BIN2CPP ${3RDPARTY_DIR}/bin2cpp/bin2cpp_mac)
else()
    set (BOOST_DIR ${3RDPARTY_DIR}/boost_linux)
    set (BIN2CPP ${3RDPARTY_DIR}/bin2cpp/bin2cpp)
endif()

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

