cmake_minimum_required(VERSION 3.15)
project(initrd NONE)

add_custom_command(OUTPUT "${PROJECT_BINARY_DIR}/initrd/init"
	COMMAND cp "${init_BINARY_DIR}/init" "${PROJECT_BINARY_DIR}/initrd/init"
	COMMENT "Installing init program"
	DEPENDS "${init_BINARY_DIR}/init")
add_custom_command(OUTPUT "${PROJECT_BINARY_DIR}/initrd"
	COMMAND cp -r "${PROJECT_SOURCE_DIR}/fs/" "${PROJECT_BINARY_DIR}/initrd"
	COMMENT "Copying files to initrd")
add_custom_command(OUTPUT "${PROJECT_BINARY_DIR}/initrd.zip"
	COMMAND zip -q -r "${PROJECT_BINARY_DIR}/initrd.zip" .
	COMMENT "Compressing initrd"
	DEPENDS "${PROJECT_BINARY_DIR}/initrd/init"
	MAIN_DEPENDENCY "${PROJECT_BINARY_DIR}/initrd"
	WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/initrd")
add_custom_target(initrd ALL DEPENDS "${PROJECT_BINARY_DIR}/initrd" "${PROJECT_BINARY_DIR}/initrd.zip")
