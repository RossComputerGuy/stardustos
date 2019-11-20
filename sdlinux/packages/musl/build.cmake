add_package(musl 1.1.24)
configure_file("${PROJECT_SOURCE_DIR}/packages/musl/config.mak.in" "${PROJECT_BINARY_DIR}/packages/musl/config.mak")