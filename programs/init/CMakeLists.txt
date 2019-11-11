cmake_minimum_required(VERSION 3.12)
project(init)

if(NOT ARCH)
  set(ARCH "x86")
endif()

add_compile_options("-nostdinc" "-fno-builtin")
set(CMAKE_EXEC_LINKER_FLAGS "-nostdlib -nostartfiles")

if(NOT CMAKE_PROJECT_NAME STREQUAL "stardustos")
  add_subdirectory(libc)
endif()
include_directories("${PROJECT_SOURCE_DIR}/libc/arch/${ARCH}/include" "${PROJECT_SOURCE_DIR}/libc/include")
include("${libc_SOURCE_DIR}/arch/${ARCH}/build.cmake")

add_executable(init "${PROJECT_SOURCE_DIR}/src/main.c")
add_dependencies(init libc)
target_link_libraries(init "${libc_BINARY_DIR}/libc.a")
