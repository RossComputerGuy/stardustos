cmake_minimum_required(VERSION 3.15)
include("platform.cmake")
# TODO: make sure we add the toolchain if its not in the system
project(hello)

add_compile_options("-nostdinc" "-fno-builtin")
add_link_options("-nostdlib" "-nostartfiles")

include("${CMAKE_BINARY_DIR}/toolchain.cmake")
if(NOT CMAKE_PROJECT_NAME STREQUAL "stardustos")
	add_subdirectory(libc)
endif()
include_directories("${PROJECT_SOURCE_DIR}/libc/arch/${ARCH}/include" "${PROJECT_SOURCE_DIR}/libc/include")
include("${libc_SOURCE_DIR}/arch/${ARCH}/build.cmake")

add_executable(hello "${PROJECT_SOURCE_DIR}/src/main.c")
add_dependencies(hello libc)
target_link_libraries(hello "${libc_BINARY_DIR}/libc.a")

if(NOT CMAKE_PROJECT_NAME STREQUAL "stardustos")
	add_subdirectory(toolchain)
	add_dependencies(hello toolchain)
endif()
