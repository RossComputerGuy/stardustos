set(PACKAGE_OUTPUT ${PROJECT_BINARY_DIR}/initrd/bin/busybox)
set(PACKAGE_DEPENDS ${PROJECT_BINARY_DIR}/initrd/lib/libc.so)
configure_file("${PROJECT_SOURCE_DIR}/packages/busybox/config.in" "${PROJECT_BINARY_DIR}/packages/busybox/config")
add_package(busybox 1.31.1)