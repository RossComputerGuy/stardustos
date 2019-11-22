cmake_minimum_required(VERSION 3.15)
include("platform.cmake")
# TODO: make sure we add the toolchain if its not in the system
project(miniz)

include("${CMAKE_BINARY_DIR}/toolchain.cmake")
include("${PROJECT_SOURCE_DIR}/build.cmake")
add_compile_options("-nostdlib" "-fno-builtin")
add_library(miniz ${MINIZ_SOURCES})

if(NOT CMAKE_PROJECT_NAME STREQUAL "stardustos")
	add_subdirectory(libc)
endif()
add_dependencies(miniz libc)
include_directories("${PROJECT_SOURCE_DIR}/libc/arch/${ARCH}/include" "${PROJECT_SOURCE_DIR}/libc/include")

if(NOT CMAKE_PROJECT_NAME STREQUAL "stardustos")
	add_subdirectory(toolchain)
	add_dependencies(miniz toolchain)
endif()
