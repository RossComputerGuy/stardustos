cmake_minimum_required(VERSION 3.12)
project(stardustos NONE)

if(NOT ARCH)
	set(ARCH "x86")
endif()

set(NEWLAND_ARCH "${ARCH}")

# Kernel stuff
add_subdirectory(newland)
add_subdirectory(modules)

# Libraries
add_subdirectory(libraries/libc)
add_subdirectory(libraries/libfile)
add_subdirectory(libraries/liblist)
add_subdirectory(libraries/miniz)

# Programs
add_subdirectory(programs/hello)
add_subdirectory(programs/init)

# Disk stuff
add_subdirectory(initrd)
add_subdirectory(rootfs)

include(CTest)
