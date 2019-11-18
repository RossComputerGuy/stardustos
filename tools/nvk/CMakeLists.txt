cmake_minimum_required(VERSION 3.15)
project(nvk)

if(NOT ARCH)
	set(ARCH "x86")
endif()

include_directories("${PROJECT_SOURCE_DIR}/newland/arch/${ARCH}/include" "${PROJECT_SOURCE_DIR}/newland/include" "${PROJECT_SOURCE_DIR}/include")
add_executable(nvk "${PROJECT_SOURCE_DIR}/src/main.c")
