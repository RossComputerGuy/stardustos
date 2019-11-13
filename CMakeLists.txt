cmake_minimum_required(VERSION 3.15)
project(stardustos NONE)

if(NOT ARCH)
	set(ARCH "x86")
endif()

set(NEWLAND_ARCH "${ARCH}")

configure_file("${PROJECT_SOURCE_DIR}/data/scripts/lint.sh.in" "${PROJECT_BINARY_DIR}/lint.sh")

add_custom_target(lint ${PROJECT_BINARY_DIR}/lint.sh)

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
