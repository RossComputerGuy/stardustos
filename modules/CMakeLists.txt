cmake_minimum_required(VERSION 3.15)
project(newland-modules)

if(NOT NEWLAND_ARCH)
	set(NEWLAND_ARCH "x86")
endif()

set(NEWLAND_SOURCE "${PROJECT_SOURCE_DIR}/newland")
include("${PROJECT_SOURCE_DIR}/newland/module.cmake")

set(libc_SOURCE_DIR "${PROJECT_SOURCE_DIR}/libc")
include("${PROJECT_SOURCE_DIR}/libc/build.cmake")

add_kmodule("misc/example.c")
