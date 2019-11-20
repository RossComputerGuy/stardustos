cmake_minimum_required(VERSION 3.15)
project(sdlinux NONE)

if(NOT CMAKE_PROJECT_NAME STREQUAL "stardustos")
	add_subdirectory(nvk)
endif()

include(pkg.cmake)

# Add packages
include(packages/busybox/build.cmake)
include(packages/linux/build.cmake)
include(packages/musl/build.cmake)