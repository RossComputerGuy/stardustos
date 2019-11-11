cmake_minimum_required(VERSION 3.12)
include("version.cmake")
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
include_directories("${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/include")

add_compile_definitions("NEWLAND_KERNEL" "MINIZ_NO_ARCHIVE_WRITING_APIS")
add_compile_options("-nostdinc" "-fno-builtin" "-Werror=implicit-function-declaration" "-fno-stack-protector")

set(libc_SOURCE_DIR "${PROJECT_SOURCE_DIR}/libc")
include("${PROJECT_SOURCE_DIR}/libc/build.cmake")

set(libfile_SOURCE_DIR "${PROJECT_SOURCE_DIR}/libfile")
include("${PROJECT_SOURCE_DIR}/libfile/build.cmake")

set(liblist_SOURCE_DIR "${PROJECT_SOURCE_DIR}/liblist")
include("${PROJECT_SOURCE_DIR}/liblist/build.cmake")

set(miniz_SOURCE_DIR "${PROJECT_SOURCE_DIR}/miniz")
set(miniz_BINARY_DIR "${PROJECT_BINARY_DIR}")
include("${PROJECT_SOURCE_DIR}/miniz/build.cmake")

configure_file("${PROJECT_SOURCE_DIR}/data/scripts/run.sh.in" "${PROJECT_BINARY_DIR}/run.sh")
configure_file("${PROJECT_SOURCE_DIR}/data/scripts/test.sh.in" "${PROJECT_BINARY_DIR}/test.sh")
configure_file("${PROJECT_SOURCE_DIR}/data/config.h.in" "${PROJECT_BINARY_DIR}/newland-config.h")
configure_file("${PROJECT_SOURCE_DIR}/data/gdbinit.in" "${PROJECT_BINARY_DIR}/.gdbinit")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXEC_LINKER_FLAGS} -T${ARCH_DIR}/linker.ld -nostdlib -nostartfiles -Wl,-Map=${PROJECT_BINARY_DIR}/kernel.map")
add_executable(kernel.elf
  ${LIBC_SOURCES}
  ${LIBFILE_SOURCES}
  ${LIBLIST_SOURCES}
  ${MINIZ_SOURCES}
  ${ARCH_FILES}
  "${ARCH_DIR}/src/alloc.c"
  "${ARCH_DIR}/src/clock.c"
  "${PROJECT_SOURCE_DIR}/src/dev/block.c"
  "${PROJECT_SOURCE_DIR}/src/dev/tty.c"
  "${PROJECT_SOURCE_DIR}/src/fs/devfs.c"
  "${PROJECT_SOURCE_DIR}/src/fs/initrd.c"
  "${PROJECT_SOURCE_DIR}/src/fs/procfs.c"
  "${PROJECT_SOURCE_DIR}/src/bus.c"
  "${PROJECT_SOURCE_DIR}/src/dev.c"
  "${PROJECT_SOURCE_DIR}/src/fs.c"
  "${PROJECT_SOURCE_DIR}/src/log.c"
  "${PROJECT_SOURCE_DIR}/src/module.c"
  "${PROJECT_SOURCE_DIR}/src/time.c")

add_test(NAME kernel
  COMMAND ${PROJECT_BINARY_DIR}/test.sh)
