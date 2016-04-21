cmake_minimum_required(VERSION 3.0.2)

# math
add_library(hxx_utils STATIC 
	math.hxx 
	log.hxx 
	callbacks.hxx
	misc.hxx
	stub.cpp)
set_target_properties(hxx_utils PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(hxx_utils
    PUBLIC
    .
    ${3RDPARTY_UNPACK_DIR}/glm
	${3RDPARTY_UNPACK_DIR}/easylogging
)
set_property (TARGET hxx_utils PROPERTY FOLDER "utils")

# add tests
add_subdirectory(test)
#${Boost_INCLUDE_DIR}
# target_link_libraries(log ${Boost_LOG_LIBRARY} ${Boost_LOG_SETUP_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
