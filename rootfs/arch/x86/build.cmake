add_custom_command(OUTPUT "${PROJECT_BINARY_DIR}/rootfs/boot/kernel.elf"
	COMMAND cp "${newland_BINARY_DIR}/kernel.elf" "${PROJECT_BINARY_DIR}/rootfs/boot/kernel.elf"
	COMMENT "Copying kernel to rootfs"
	DEPENDS "${newland_BINARY_DIR}/kernel.elf")
add_custom_command(OUTPUT "${PROJECT_BINARY_DIR}/rootfs/boot/initrd.zip"
	COMMAND cp "${initrd_BINARY_DIR}/initrd.zip" "${PROJECT_BINARY_DIR}/rootfs/boot/initrd.zip"
	COMMENT "Copying initrd to rootfs"
	DEPENDS "${initrd_BINARY_DIR}/initrd.zip")
