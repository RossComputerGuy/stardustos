cmake_minimum_required(VERSION 3.15)
include("platform.cmake")
project(nvk)

set(NEWLAND_ARCH ${ARCH})
include_directories("${PROJECT_SOURCE_DIR}/include")

set(newland_SOURCE_DIR "${PROJECT_SOURCE_DIR}/newland")
include("${PROJECT_SOURCE_DIR}/newland/build.cmake")

option(HOST_UNICORN "Use the host version of the Unicorn Engine" ON)

configure_file("${PROJECT_SOURCE_DIR}/data/scripts/genlinker.sh.in" "${PROJECT_BINARY_DIR}/genlinker.sh")
execute_process(COMMAND "${PROJECT_BINARY_DIR}/genlinker.sh")

find_package(PkgConfig)
find_package(Threads REQUIRED)

if(PKG_CONFIG_FOUND AND HOST_UNICORN)
	pkg_check_modules(UNICORN REQUIRED unicorn)
else()
	find_library(UNICORN unicorn)
	if(UNICORN STREQUAL "UNICORN-NOTFOUND")
		set(UNICORN_HAS_ARM OFF)
		set(UNICORN_HAS_AARCH64 OFF)
		set(UNICORN_HAS_M68K OFF)
		set(UNICORN_HAS_MIPS OFF)
		set(UNICORN_HAS_SPARC OFF)
		add_subdirectory(deps/unicorn)
		include_directories("${unicorn_SOURCE_DIR}/include")
	endif()
endif()

add_compile_definitions("__nvk__" "HAS_LIBC")
add_link_options("-T${PROJECT_BINARY_DIR}/nvk.ld")
add_executable(nvk
	${NEWLAND_SOURCES}
	"${PROJECT_SOURCE_DIR}/src/fs/rootfs.c"
	"${PROJECT_SOURCE_DIR}/src/emulator.c"
	"${PROJECT_SOURCE_DIR}/src/main.c"
	"${PROJECT_SOURCE_DIR}/src/proc.c"
	"${PROJECT_SOURCE_DIR}/src/time.c")
target_link_libraries(nvk ${CMAKE_THREAD_LIBS_INIT})
if(PKG_CONFIG_FOUND AND HOST_UNICORN)
	target_link_libraries(nvk ${UNICORN_LIBRARIES})
	target_include_directories(nvk PUBLIC ${UNICORN_INCLUDE_DIRS})
	target_compile_options(nvk PUBLIC ${UNICORN_CFLAGS_OTHER})
else()
	if(UNICORN STREQUAL "UNICORN-NOTFOUND")
		add_dependencies(nvk unicorn)
		target_link_libraries(nvk ${unicorn_BINARY_DIR}/libunicorn.so)
	else()
		string(REPLACE ".so" ".a" UNICORN ${UNICORN})
		target_link_libraries(nvk ${UNICORN})
	endif()
endif()