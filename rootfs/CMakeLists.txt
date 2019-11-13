cmake_minimum_required(VERSION 3.15)
project(rootfs NONE)

if(NOT CMAKE_PROJECT_NAME STREQUAL "stardustos")
	add_subdirectory(newland)
	add_subdirectory(initrd)
endif()

if(NOT ARCH)
	set(ARCH x86)
	if(NOT PLATFORM)
		set(PLATFORM pc)
	endif()
endif()

if(ARCH STREQUAL "x86" AND NOT PLATFORM)
	set(PLATFORM pc)
endif()

include("${PROJECT_SOURCE_DIR}/arch/${ARCH}/build.cmake")
include("${PROJECT_SOURCE_DIR}/arch/${ARCH}/platform/${PLATFORM}/build.cmake")

configure_file("${PROJECT_SOURCE_DIR}/data/scripts/copyrootfs.sh.in" "${PROJECT_BINARY_DIR}/copyrootfs.sh")

add_custom_command(OUTPUT "${PROJECT_BINARY_DIR}/rootfs/"
	COMMAND sh ${PROJECT_BINARY_DIR}/copyrootfs.sh
	COMMENT "Copying files to rootfs")
add_custom_target(rootfs ALL DEPENDS "${PROJECT_BINARY_DIR}/rootfs" "${PROJECT_BINARY_DIR}/rootfs.iso")
