cmake_minimum_required(VERSION 3.12)
project(newland-modules)

if(NOT NEWLAND_ARCH)
  set(NEWLAND_ARCH "x86")
endif()
set(NEWLAND_SOURCE "${PROJECT_SOURCE_DIR}/newland")
include("${PROJECT_SOURCE_DIR}/newland/module.cmake")

add_kmodule("misc/example.c")
