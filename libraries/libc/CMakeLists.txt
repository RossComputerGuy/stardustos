cmake_minimum_required(VERSION 3.12)
project(libc C ASM)

if(NOT ARCH)
	set(ARCH "x86")
endif()

include("${PROJECT_SOURCE_DIR}/build.cmake")
include("${PROJECT_SOURCE_DIR}/arch/${ARCH}/build.cmake")
add_compile_options("-nostdlib" "-fno-builtin" "-fno-stack-protector")
add_library(libc ${LIBC_SOURCES})
set_target_properties(libc PROPERTIES PREFIX "")
