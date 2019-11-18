cmake_minimum_required(VERSION 3.15)
project(nvk)

if(NOT ARCH)
	set(ARCH "x86")
endif()

set(NEWLAND_ARCH ${ARCH})
include_directories("${PROJECT_SOURCE_DIR}/include")

set(newland_SOURCE_DIR "${PROJECT_SOURCE_DIR}/newland")
include("${PROJECT_SOURCE_DIR}/newland/build.cmake")

find_package(Threads REQUIRED)

add_compile_definitions("__nvk__")
add_link_options("${CMAKE_THREAD_LIBS_INIT}")
add_executable(nvk
	${NEWLAND_SOURCES}
	"${PROJECT_SOURCE_DIR}/src/main.c"
	"${PROJECT_SOURCE_DIR}/src/proc.c"
	"${PROJECT_SOURCE_DIR}/src/time.c")
