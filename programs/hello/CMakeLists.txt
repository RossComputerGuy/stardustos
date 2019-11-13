cmake_minimum_required(VERSION 3.15)
project(hello)

if(NOT ARCH)
	set(ARCH "x86")
endif()

add_compile_options("-nostdinc" "-fno-builtin")
add_link_options("-nostdlib" "-nostartfiles")

if(NOT CMAKE_PROJECT_NAME STREQUAL "stardustos")
	add_subdirectory(libc)
endif()
include_directories("${PROJECT_SOURCE_DIR}/libc/arch/${ARCH}/include" "${PROJECT_SOURCE_DIR}/libc/include")
include("${libc_SOURCE_DIR}/arch/${ARCH}/build.cmake")

add_executable(hello "${PROJECT_SOURCE_DIR}/src/main.c")
add_dependencies(hello libc)
target_link_libraries(hello "${libc_BINARY_DIR}/libc.a")
