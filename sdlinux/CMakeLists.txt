cmake_minimum_required(VERSION 3.15)
# TODO: make sure we add the toolchain if its not in the system
project(sdlinux NONE)

include("${CMAKE_BINARY_DIR}/toolchain.cmake")
if(NOT CMAKE_PROJECT_NAME STREQUAL "stardustos")
	add_subdirectory(nvk)
endif()

configure_file("${PROJECT_SOURCE_DIR}/data/scripts/geninitrd.sh.in" "${PROJECT_BINARY_DIR}/geninitrd.sh" @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/data/scripts/toolchain.sh.in" "${PROJECT_BINARY_DIR}/toolchain.sh" @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/data/toolchain.cmake.in" "${PROJECT_BINARY_DIR}/toolchain.cmake" @ONLY)
include(pkg.cmake)

# Add packages
include(packages/linux/build.cmake)
include(packages/musl/build.cmake)
include(packages/unicorn/build.cmake)
include(packages/busybox/build.cmake)
include(packages/nvk/build.cmake)

add_custom_command(OUTPUT "${PROJECT_BINARY_DIR}/rootfs/boot/initrd.cpio"
	COMMAND "${PROJECT_BINARY_DIR}/geninitrd.sh"
	WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/initrd"
	COMMENT "Building initrd"
	DEPENDS ${PROJECT_BINARY_DIR}/initrd/bin/busybox ${PROJECT_BINARY_DIR}/initrd/bin/nvk)
add_custom_target(sdlinux-initrd ALL DEPENDS "${PROJECT_BINARY_DIR}/rootfs/boot/initrd.cpio")