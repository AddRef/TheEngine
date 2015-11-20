cmake_minimum_required(VERSION 3.0.2)

file(GLOB PUBLIC_HEADERS "include/*.h")
file(GLOB SOURCE "src/*.h" "*.cpp")

add_library(log STATIC 
	${PUBLIC_HEADERS}
	${SOURCE}
)
target_include_directories(log 
	PUBLIC
	./include
	${3RDPARTY_UNPACK_DIR}/easylogging
	)

#${Boost_INCLUDE_DIR}
# target_link_libraries(log ${Boost_LOG_LIBRARY} ${Boost_LOG_SETUP_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})