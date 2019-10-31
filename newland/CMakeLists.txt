cmake_minimum_required(VERSION 3.15)
project(newland C ASM)

if(NOT TARGET_ARCH)
  set(TARGET_ARCH x86)
endif()

set(ARCH_DIR "${PROJECT_SOURCE_DIR}/arch/${TARGET_ARCH}")

include("${ARCH_DIR}/build.cmake")
include_directories("${PROJECT_SOURCE_DIR}/include")

add_compile_options("-nostdinc" "-fno-builtin" "-Werror=implicit-function-declaration" "-fno-stack-protector")
add_link_options("-T${ARCH_DIR}/linker.ld" "-nostdlib" "-nostartfiles")
add_executable(kernel.elf ${ARCH_FILES}
  "${ARCH_DIR}/src/kalloc.c"
  "${PROJECT_SOURCE_DIR}/src/dev.c"
  "${PROJECT_SOURCE_DIR}/src/string.c")
