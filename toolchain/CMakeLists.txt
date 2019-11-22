cmake_minimum_required(VERSION 3.15)
project(toolchain NONE)

function(add_prog name output)
	configure_file("${PROJECT_SOURCE_DIR}/data/scripts/${name}.sh.in" "${PROJECT_BINARY_DIR}/${name}.sh" @ONLY)
	add_custom_command(OUTPUT ${output}
		COMMAND "${PROJECT_BINARY_DIR}/${name}.sh"
		COMMENT "Building toolchain package: ${name}")
	add_custom_target(toolchain-${name} ALL DEPENDS ${PROG_DEPENDS} ${output})
endfunction()

include(platform.cmake)
configure_file("${PROJECT_SOURCE_DIR}/data/toolchain.cmake.in" "${CMAKE_BINARY_DIR}/toolchain.cmake" @ONLY)
add_prog(binutils ${PROJECT_BINARY_DIR}/crossroot/bin/as)
add_prog(gcc ${PROJECT_BINARY_DIR}/crossroot/bin/gcc)
add_custom_target(toolchain ALL DEPENDS ${PROJECT_BINARY_DIR}/crossroot/bin/as ${PROJECT_BINARY_DIR}/crossroot/bin/gcc)
