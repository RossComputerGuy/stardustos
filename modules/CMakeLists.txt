cmake_minimum_required(VERSION 3.15)
project(newland-modules)

if(NOT NEWLAND_ARCH)
  set(NEWLAND_ARCH "x86")
endif()
set(NEWLAND_SOURCE "${PROJECT_SOURCE_DIR}/newland")
include("${PROJECT_SOURCE_DIR}/newland/module.cmake")

add_kmodule(example "${PROJECT_SOURCE_DIR}/misc/example.c")
