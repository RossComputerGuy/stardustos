set(PACKAGE_OUTPUT ${PROJECT_BINARY_DIR}/initrd/lib/libunicorn.so)
set(PACKAGE_DEPENDS ${PROJECT_BINARY_DIR}/buildfs/lib/libc.a)
add_package(unicorn 1.0.2-rc1)