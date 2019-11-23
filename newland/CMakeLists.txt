cmake_minimum_required(VERSION 3.15)
include("version.cmake")
include("platform.cmake")
project(newland C ASM)

if(NOT NEWLAND_ARCH)
	set(NEWLAND_ARCH ${ARCH})
endif()

if(NOT NEWLAND_KLOG_SIZE)
	set(NEWLAND_KLOG_SIZE 1000)
endif()

include("${CMAKE_BINARY_DIR}/toolchain.cmake")
set(ARCH ${NEWLAND_ARCH})
add_compile_options("-nostdinc" "-fno-builtin" "-fno-stack-protector")
include(CTest)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_options("-fdump-rtl-expand")
	add_link_options("-Wl,-Map=${PROJECT_BINARY_DIR}/kernel.map")
endif()

set(libc_SOURCE_DIR "${PROJECT_SOURCE_DIR}/libc")
include("${PROJECT_SOURCE_DIR}/libc/build.cmake")
include("${PROJECT_SOURCE_DIR}/build.cmake")

configure_file("${PROJECT_SOURCE_DIR}/data/scripts/buildman.sh.in" "${PROJECT_BINARY_DIR}/buildman.sh" @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/data/scripts/lint.sh.in" "${PROJECT_BINARY_DIR}/lint.sh" @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/data/scripts/mkcallgraph.sh.in" "${PROJECT_BINARY_DIR}/mkcallgraph.sh" @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/data/scripts/run.sh.in" "${PROJECT_BINARY_DIR}/run.sh" @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/data/scripts/test.sh.in" "${PROJECT_BINARY_DIR}/test.sh" @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/data/scripts/uploadman.sh.in" "${PROJECT_BINARY_DIR}/uploadman.sh" @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/data/gdbinit.in" "${PROJECT_BINARY_DIR}/.gdbinit")

if(NOT CMAKE_PROJECT_NAME STREQUAL "stardustos")
	add_custom_target(lint ${PROJECT_BINARY_DIR}/lint.sh)
	add_custom_target(man ALL ${PROJECT_BINARY_DIR}/buildman.sh)
endif()

find_package(Doxygen REQUIRED dot)
set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/docs)
set(DOXYGEN_RECURSIVE TRUE)
set(DOXYGEN_CALL_GRAPH TRUE)
set(DOXYGEN_CALLER_GRAPH TRUE)
set(DOXYGEN_TAB_SIZE 2)
set(DOXYGEN_EXTRACT_ALL TRUE)
set(DOXYGEN_SOURCE_BROWSER TRUE)
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${PROJECT_SOURCE_DIR}/README.md)
set(DOXYGEN_QUIET TRUE)
set(DOXYGEN_EXCLUDE ${PROJECT_SOURCE_DIR}/toolchain)
doxygen_add_docs(docs ALL ${PROJECT_SOURCE_DIR}/arch ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/libc ${PROJECT_SOURCE_DIR}/libfile ${PROJECT_SOURCE_DIR}/liblist ${PROJECT_SOURCE_DIR}/miniz ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/README.md
	${PROJECT_BINARY_DIR}/manual.md
	COMMENT "Building kernel documentation")

add_link_options("-T${ARCH_DIR}/linker.ld" "-nostdlib" "-nostartfiles")
add_executable(kernel.elf
	${LIBC_SOURCES}
	${ARCH_FILES}
	${NEWLAND_SOURCES}
	"${PROJECT_SOURCE_DIR}/deps/liballoc/liballoc.c"
	"${ARCH_DIR}/src/alloc.c"
	"${ARCH_DIR}/src/clock.c")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_custom_command(OUTPUT "${PROJECT_BINARY_DIR}/callgraph.png"
		COMMAND ${PROJECT_BINARY_DIR}/mkcallgraph.sh
		COMMENT "Generating function call graph")
	add_custom_target(kernel-callgraph ALL DEPENDS "${PROJECT_BINARY_DIR}/callgraph.png")
endif()

add_test(NAME kernel
	COMMAND ${PROJECT_BINARY_DIR}/test.sh)

if(NOT CMAKE_PROJECT_NAME STREQUAL "stardustos")
	add_subdirectory(toolchain)
	add_dependencies(kernel.elf toolchain)
endif()
