cmake_minimum_required(VERSION 3.12)
project(liblist)

if(NOT ARCH)
  set(ARCH "x86")
endif()

include("${PROJECT_SOURCE_DIR}/build.cmake")
add_compile_options("-nostdlib" "-fno-builtin")
add_library(liblist ${LIBLIST_SOURCES})
set_target_properties(liblist PROPERTIES PREFIX "")

if(NOT CMAKE_PROJECT_NAME STREQUAL "stardustos")
  add_subdirectory(libc)
endif()
add_dependencies(liblist libc)
include_directories("${PROJECT_SOURCE_DIR}/libc/arch/${ARCH}/include" "${PROJECT_SOURCE_DIR}/libc/include")
