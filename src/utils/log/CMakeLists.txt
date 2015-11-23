cmake_minimum_required(VERSION 3.0.2)

file(GLOB HEADERS "*.h")
file(GLOB SOURCE "*.cpp")

add_library(log STATIC 
	${HEADERS}
	${SOURCE}
)
target_include_directories(log 
	PUBLIC
	.
	${3RDPARTY_UNPACK_DIR}/easylogging
)

#${Boost_INCLUDE_DIR}
# target_link_libraries(log ${Boost_LOG_LIBRARY} ${Boost_LOG_SETUP_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})