set(PACKAGE_OUTPUT ${PROJECT_BINARY_DIR}/initrd/lib/libc.so)
set(PACKAGE_DEPENDS ${PROJECT_BINARY_DIR}/rootfs/boot/vmlinuz)
configure_file("${PROJECT_SOURCE_DIR}/packages/musl/musl-gcc.specs.in" "${PROJECT_BINARY_DIR}/packages/musl/musl-gcc.specs" @ONLY)
add_package(musl 1.1.24)