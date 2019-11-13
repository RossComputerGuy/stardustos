include_directories("${libc_SOURCE_DIR}/arch/${ARCH}/include" "${libc_SOURCE_DIR}/include")
set(LIBC_SOURCES "${libc_SOURCE_DIR}/src/lib.c"
# libgen
	"${libc_SOURCE_DIR}/src/libgen/basename.c" "${libc_SOURCE_DIR}/src/libgen/dirname.c"
# math
	"${libc_SOURCE_DIR}/src/math/arithm.c" "${libc_SOURCE_DIR}/src/math/pow.c"
# stdlib
	"${libc_SOURCE_DIR}/src/stdlib/ftoa.c" "${libc_SOURCE_DIR}/src/stdlib/ultoa.c"
# stdio
	"${libc_SOURCE_DIR}/src/stdio/__file_write.c" "${libc_SOURCE_DIR}/src/stdio/__file_read.c"
	"${libc_SOURCE_DIR}/src/stdio/printf.c" "${libc_SOURCE_DIR}/src/stdio/fprintf.c" "${libc_SOURCE_DIR}/src/stdio/snprintf.c" "${libc_SOURCE_DIR}/src/stdio/vfprintf.c" "${libc_SOURCE_DIR}/src/stdio/vsnprintf.c"
# string
	"${libc_SOURCE_DIR}/src/string/itoa.c"
	"${libc_SOURCE_DIR}/src/string/memcmp.c" "${libc_SOURCE_DIR}/src/string/memcpy.c" "${libc_SOURCE_DIR}/src/string/memmove.c" "${libc_SOURCE_DIR}/src/string/memset.c"
	"${libc_SOURCE_DIR}/src/string/strcmp.c" "${libc_SOURCE_DIR}/src/string/strcpy.c" "${libc_SOURCE_DIR}/src/string/strlen.c" "${libc_SOURCE_DIR}/src/string/strncmp.c" "${libc_SOURCE_DIR}/src/string/strncpy.c")
if(NOT PROJECT_NAME STREQUAL "newland")
	list(APPEND LIBC_SOURCES "${libc_SOURCE_DIR}/arch/${ARCH}/src/asm/crt0.s" "${libc_SOURCE_DIR}/arch/${ARCH}/src/asm/crtn.s" "${libc_SOURCE_DIR}/arch/${ARCH}/src/asm/entry.s"
# stdlib
		"${libc_SOURCE_DIR}/src/stdlib/free.c" "${libc_SOURCE_DIR}/src/stdlib/malloc.c")
endif()
