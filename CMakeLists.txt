cmake_minimum_required(VERSION 3.15)

include(platform.cmake)
add_subdirectory(toolchain)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/toolchain.cmake)
project(stardustos NONE)

set(NEWLAND_ARCH "${ARCH}")

configure_file("${PROJECT_SOURCE_DIR}/data/scripts/lint.sh.in" "${PROJECT_BINARY_DIR}/lint.sh")

add_custom_target(lint ${PROJECT_BINARY_DIR}/lint.sh)

# Tools
add_subdirectory(tools/nvk)

# Kernel stuff
add_subdirectory(newland)
add_subdirectory(modules)

add_custom_target(man ALL ${newland_BINARY_DIR}/buildman.sh)

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

# Misc
add_subdirectory(sdlinux)

include(CTest)
