# Set global build variables
set (SRC_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
# Set output folder
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_64 TRUE) 
else() 
    set(ARCH_64 FALSE) 
endif()

if (WIN32)
    set (DYNAMIC_LIB_SUFIX dll)
    set (STATIC_LIB_SUFIX lib)
elseif(APPLE)
    set (DYNAMIC_LIB_SUFIX dylib)
    set (STATIC_LIB_SUFIX a)
else()
    set (DYNAMIC_LIB_SUFIX so)
    set (STATIC_LIB_SUFIX a)
endif()