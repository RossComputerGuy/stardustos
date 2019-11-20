# Package builder

function(add_package name version)
	configure_file("${PROJECT_SOURCE_DIR}/packages/${name}/build.sh.in" "${PROJECT_BINARY_DIR}/${name}.pkg.sh")
	add_custom_target(sdlinux-${name} ALL
		COMMAND "${PROJECT_BINARY_DIR}/${name}.pkg.sh"
		COMMENT "Building package ${name} (v${version})")
endfunction()
