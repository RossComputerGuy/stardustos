cmake_minimum_required(VERSION 3.12)
project(stardustos NONE)

if(NOT ARCH)
  set(ARCH "x86")
endif()

set(NEWLAND_ARCH "${ARCH}")
add_subdirectory(newland)
add_subdirectory(modules)
add_subdirectory(libc)
add_subdirectory(libfile)
add_subdirectory(liblist)
add_subdirectory(miniz)

include(CTest)
