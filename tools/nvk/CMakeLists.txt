cmake_minimum_required(VERSION 3.15)
project(nvk)

if(NOT ARCH)
	set(ARCH "x86")
endif()

set(NEWLAND_ARCH ${ARCH})
include_directories("${PROJECT_SOURCE_DIR}/include")

set(newland_SOURCE_DIR "${PROJECT_SOURCE_DIR}/newland")
include("${PROJECT_SOURCE_DIR}/newland/build.cmake")

find_package(PkgConfig)
find_package(Threads REQUIRED)

if(PKG_CONFIG_FOUND)
	pkg_check_modules(UNICORN REQUIRED unicorn)
else()
	add_subdirectory(deps/unicorn)
	include_directories("${unicorn_SOURCE_DIR}/include")
endif()

add_compile_definitions("__nvk__")
add_executable(nvk
	${NEWLAND_SOURCES}
	"${PROJECT_SOURCE_DIR}/src/emulator.c"
	"${PROJECT_SOURCE_DIR}/src/main.c"
	"${PROJECT_SOURCE_DIR}/src/proc.c"
	"${PROJECT_SOURCE_DIR}/src/time.c")
target_link_libraries(nvk ${CMAKE_THREAD_LIBS_INIT})
if(PKG_CONFIG_FOUND)
	target_link_libraries(nvk ${UNICORN_LIBRARIES})
	target_include_directories(nvk PUBLIC ${UNICORN_INCLUDE_DIRS})
	target_compile_options(nvk PUBLIC ${UNICORN_CFLAGS_OTHER})
else()
	add_dependencies(nvk unicorn)
	target_link_libraries(nvk ${unicorn_BINARY_DIR}/libunicorn.so)
endif()