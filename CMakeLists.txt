cmake_minimum_required(VERSION 3.12)
project(newland NONE)

add_subdirectory(newland)
add_subdirectory(modules)
add_subdirectory(libc)

include(CTest)
