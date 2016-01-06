cmake_minimum_required(VERSION 3.0.2)

# math
add_library(math STATIC math.hxx stub.cpp)
set_target_properties(math PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(math
    PUBLIC
    .
    ${3RDPARTY_UNPACK_DIR}/glm
)
set_property (TARGET math PROPERTY FOLDER "utils")
# log
add_library(log STATIC log.hxx stub.cpp)
set_target_properties(log PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(log 
	PUBLIC
	.
	${3RDPARTY_UNPACK_DIR}/easylogging
)
set_property (TARGET log PROPERTY FOLDER "utils")
# callbacks
add_library(callbacks STATIC callbacks.hxx stub.cpp)
set_target_properties(callbacks PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(callbacks 
	PUBLIC
	.
)
set_property (TARGET callbacks PROPERTY FOLDER "utils")
# misc
add_library(misc STATIC misc.hxx stub.cpp)
set_target_properties(misc PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(misc 
	PUBLIC
	.
)
set_property (TARGET misc PROPERTY FOLDER "utils")
# add tests
add_subdirectory(test)
#${Boost_INCLUDE_DIR}
# target_link_libraries(log ${Boost_LOG_LIBRARY} ${Boost_LOG_SETUP_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
