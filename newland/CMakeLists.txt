cmake_minimum_required(VERSION 3.12)
project(newland C ASM)

if(NOT NEWLAND_ARCH)
  set(NEWLAND_ARCH x86)
endif()

if(NOT NEWLAND_KLOG_SIZE)
  set(NEWLAND_KLOG_SIZE 1000)
endif()

set(ARCH_DIR "${PROJECT_SOURCE_DIR}/arch/${NEWLAND_ARCH}")

include(CTest)
include("${ARCH_DIR}/build.cmake")
include_directories("${PROJECT_SOURCE_DIR}/include")

configure_file("${PROJECT_SOURCE_DIR}/data/gdbinit.in" "${PROJECT_BINARY_DIR}/.gdbinit")

add_compile_definitions("NEWLAND_KLOG_SIZE=${NEWLAND_KLOG_SIZE}")
add_compile_options("-nostdinc" "-fno-builtin" "-Werror=implicit-function-declaration" "-fno-stack-protector")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -T${ARCH_DIR}/linker.ld -nostdlib -nostartfiles")
add_executable(kernel.elf ${ARCH_FILES}
  "${ARCH_DIR}/src/clock.c"
  "${ARCH_DIR}/src/kalloc.c"
  "${PROJECT_SOURCE_DIR}/src/dev/block.c"
  "${PROJECT_SOURCE_DIR}/src/dev/tty.c"
  "${PROJECT_SOURCE_DIR}/src/bus.c"
  "${PROJECT_SOURCE_DIR}/src/dev.c"
  "${PROJECT_SOURCE_DIR}/src/fs.c"
  "${PROJECT_SOURCE_DIR}/src/log.c"
  "${PROJECT_SOURCE_DIR}/src/module.c"
  "${PROJECT_SOURCE_DIR}/src/string.c"
  "${PROJECT_SOURCE_DIR}/src/time.c")

find_program(EMULATOR ${EMULATOR})
add_test(NAME kernel
  COMMAND ${PROJECT_SOURCE_DIR}/scripts/test.sh ${EMULATOR} ${PROJECT_BINARY_DIR})