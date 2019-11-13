add_custom_command(OUTPUT "${PROJECT_BINARY_DIR}/rootfs.iso"
	COMMAND grub-mkrescue -o "${PROJECT_BINARY_DIR}/rootfs.iso" "${PROJECT_BINARY_DIR}/rootfs" --product-name=StardustOS --product-version="v0.1.0"
	COMMENT "Building ISO"
	MAIN_DEPENDENCY "${PROJECT_BINARY_DIR}/rootfs"
	DEPENDS "${PROJECT_BINARY_DIR}/rootfs/boot/kernel.elf" "${PROJECT_BINARY_DIR}/rootfs/boot/initrd.zip")
