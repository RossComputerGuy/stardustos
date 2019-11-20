set(PACKAGE_OUTPUT ${PROJECT_BINARY_DIR}/initrd/lib/libc.a)
set(PACKAGE_DEPENDS ${PROJECT_BINARY_DIR}/rootfs/boot/vmlinuz)
add_package(musl 1.1.24)