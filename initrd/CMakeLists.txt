cmake_minimum_required(VERSION 3.12)
project(initrd NONE)

add_custom_command(OUTPUT "${PROJECT_BINARY_DIR}/initrd"
  COMMAND cp -r "${PROJECT_SOURCE_DIR}/fs/" "${PROJECT_BINARY_DIR}/initrd"
  COMMENT "Copying files to initrd")
add_custom_command(OUTPUT "${PROJECT_BINARY_DIR}/initrd.zip"
  COMMAND zip -r "${PROJECT_BINARY_DIR}/initrd.zip" .
  COMMENT "Compressing initrd"
  MAIN_DEPENDENCY "${PROJECT_BINARY_DIR}/initrd"
  WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/initrd")
add_custom_target(initrd ALL DEPENDS "${PROJECT_BINARY_DIR}/initrd" "${PROJECT_BINARY_DIR}/initrd.zip")
