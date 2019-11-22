cmake_minimum_required(VERSION 3.15)
include("platform.cmake")
# TODO: make sure we add the toolchain if its not in the system
project(libc C ASM)

include("${CMAKE_BINARY_DIR}/toolchain.cmake")
include("${PROJECT_SOURCE_DIR}/build.cmake")
include("${PROJECT_SOURCE_DIR}/arch/${ARCH}/build.cmake")
add_compile_options("-nostdlib" "-fno-builtin" "-fno-stack-protector")
add_library(libc ${LIBC_SOURCES})
set_target_properties(libc PROPERTIES PREFIX "")

if(NOT CMAKE_PROJECT_NAME STREQUAL "stardustos")
	add_subdirectory(toolchain)
	add_dependencies(libc toolchain)
endif()
