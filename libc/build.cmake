include_directories("${libc_SOURCE_DIR}/include")
set(LIBC_SOURCES
# stdlib
  "${libc_SOURCE_DIR}/src/stdlib/ultoa.c"
# string
  "${libc_SOURCE_DIR}/src/string/itoa.c"
  "${libc_SOURCE_DIR}/src/string/memcpy.c" "${libc_SOURCE_DIR}/src/string/memmove.c" "${libc_SOURCE_DIR}/src/string/memset.c"
  "${libc_SOURCE_DIR}/src/string/strcmp.c" "${libc_SOURCE_DIR}/src/string/strcpy.c" "${libc_SOURCE_DIR}/src/string/strlen.c" "${libc_SOURCE_DIR}/src/string/strncmp.c" "${libc_SOURCE_DIR}/src/string/strncpy.c")
