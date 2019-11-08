cmake_minimum_required(VERSION 3.12)
project(libfile)

if(NOT ARCH)
  set(ARCH "x86")
endif()

include("${PROJECT_SOURCE_DIR}/build.cmake")
add_compile_options("-nostdlib" "-fno-builtin")
add_library(libfile ${LIBFILE_SOURCES})
set_target_properties(libfile PROPERTIES PREFIX "")

if(NOT CMAKE_PROJECT_NAME STREQUAL "stardustos")
  add_subdirectory(libc)
endif()
add_dependencies(libfile libc)
include_directories("${PROJECT_SOURCE_DIR}/libc/arch/${ARCH}/include" "${PROJECT_SOURCE_DIR}/libc/include")