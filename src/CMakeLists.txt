cmake_minimum_required(VERSION 3.14)
project(HIDDataInterpreterLib VERSION 1.0.0)

set (CMAKE_CXX_STANDARD 11)

#List of sources
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)

file(GLOB HEADERS_FILES ${PROJECT_SOURCE_DIR}/../inc/*.h)

if (WIN32)
	#Add filters in visual studios
	source_group("src"          FILES ${SRC_FILES})
	source_group("headers"      FILES ${HEADERS_FILES} )

	#Remove zero check project
	set(CMAKE_SUPPRESS_REGENERATION true)
endif()

add_library(HIDDataInterpreterLib ${SRC_FILES} ${HEADERS_FILES})

target_include_directories(HIDDataInterpreterLib PUBLIC ${PROJECT_SOURCE_DIR}/../inc)
