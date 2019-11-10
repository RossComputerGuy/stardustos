add_custom_command(OUTPUT "${PROJECT_BINARY_DIR}/rootfs/boot/kernel.elf"
  COMMAND cp "${newland_BINARY_DIR}/kernel" "${PROJECT_BINARY_DIR}/rootfs/boot/kernel.elf"
  COMMENT "Copying kernel to rootfs"
  MAIN_DEPENDENCY newland)
add_custom_command(OUTPUT "${PROJECT_BINARY_DIR}/rootfs/boot/initrd.zip"
  COMMAND cp "${initrd_BINARY_DIR}/initrd.zip" "${PROJECT_BINARY_DIR}/rootfs/boot/initrd.zip"
  COMMENT "Copying initrd to rootfs"
  MAIN_DEPENDENCY initrd)
