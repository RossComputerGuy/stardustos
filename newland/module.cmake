# Kernel Module CMake Build Script

if(NOT NEWLAND_SOURCE)
	message(FATAL_ERROR "Newland kernel build script requires variable to be defined: NEWLAND_SOURCE")
endif()

if(NOT NEWLAND_ARCH)
	message(FATAL_ERROR "Newland kernel build script requires variable to be defined: NEWLAND_ARCH")
endif()

include("${NEWLAND_SOURCE}/version.cmake")
include("${NEWLAND_SOURCE}/arch/${NEWLAND_ARCH}/build.cmake")
if(NOT CMAKE_PROJECT_NAME STREQUAL "stardustos")
	add_subdirectory("${NEWLAND_SOURCE}/toolchain")
endif()

function(add_kmodule name)
	include_directories("${NEWLAND_SOURCE}/include" "${NEWLAND_SOURCE}/arch/${NEWLAND_ARCH}/include" "${NEWLAND_SOURCE}/liblist/include")
	add_compile_definitions("NEWLAND_MODULE" "NEWLAND_VERSION=\"${NEWLAND_VERSION}\"")
	add_compile_options("-nostdinc" "-fno-builtin" "-Werror=implicit-function-declaration" "-fno-stack-protector" "-r")
	string(REPLACE "/" "_" "target" "${name}")
	string(REPLACE ".c" ".o" "target" "${target}")
	add_library("${target}" OBJECT "${name}")
	set_target_properties("${target}" PROPERTIES PREFIX "")
	if(NOT CMAKE_PROJECT_NAME STREQUAL "stardustos")
		add_dependencies("${target}" toolchain)
	endif()
endfunction()
