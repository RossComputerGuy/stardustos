set(ARCH_DIR "${newland_SOURCE_DIR}/arch/${NEWLAND_ARCH}")
include("${ARCH_DIR}/build.cmake")
include_directories("${PROJECT_BINARY_DIR}" "${newland_SOURCE_DIR}/include")

configure_file("${newland_SOURCE_DIR}/data/config.h.in" "${PROJECT_BINARY_DIR}/newland-config.h")

add_compile_definitions("NEWLAND_KERNEL" "MINIZ_NO_ARCHIVE_WRITING_APIS")
add_compile_options("-Werror=implicit-function-declaration")

# Dependencies
set(libfile_SOURCE_DIR "${newland_SOURCE_DIR}/libfile")
include("${newland_SOURCE_DIR}/libfile/build.cmake")

set(liblist_SOURCE_DIR "${newland_SOURCE_DIR}/liblist")
include("${newland_SOURCE_DIR}/liblist/build.cmake")

set(miniz_SOURCE_DIR "${newland_SOURCE_DIR}/miniz")
include("${newland_SOURCE_DIR}/miniz/build.cmake")

set(NEWLAND_SOURCES ${LIBFILE_SOURCES}
	${LIBLIST_SOURCES}
	${MINIZ_SOURCES}
	"${newland_SOURCE_DIR}/src/dev/block.c"
	"${newland_SOURCE_DIR}/src/dev/tty.c"
	"${newland_SOURCE_DIR}/src/fs/devfs.c"
	"${newland_SOURCE_DIR}/src/fs/initrd.c"
	"${newland_SOURCE_DIR}/src/fs/procfs.c"
	"${newland_SOURCE_DIR}/src/bus.c"
	"${newland_SOURCE_DIR}/src/dev.c"
	"${newland_SOURCE_DIR}/src/fs.c"
	"${newland_SOURCE_DIR}/src/log.c"
	"${newland_SOURCE_DIR}/src/module.c"
	"${newland_SOURCE_DIR}/src/syscall.c"
	"${newland_SOURCE_DIR}/src/system.c"
	"${newland_SOURCE_DIR}/src/time.c")
