include_directories("${libc_SOURCE_DIR}/include")
set(LIBC_SOURCES
# math
  "${libc_SOURCE_DIR}/src/math/pow.c"
# stdlib
  "${libc_SOURCE_DIR}/src/stdlib/ftoa.c" "${libc_SOURCE_DIR}/src/stdlib/ultoa.c"
# stdio
  "${libc_SOURCE_DIR}/src/stdio/vsnprintf.c"
# string
  "${libc_SOURCE_DIR}/src/string/itoa.c"
  "${libc_SOURCE_DIR}/src/string/memcpy.c" "${libc_SOURCE_DIR}/src/string/memmove.c" "${libc_SOURCE_DIR}/src/string/memset.c"
  "${libc_SOURCE_DIR}/src/string/strcmp.c" "${libc_SOURCE_DIR}/src/string/strcpy.c" "${libc_SOURCE_DIR}/src/string/strlen.c" "${libc_SOURCE_DIR}/src/string/strncmp.c" "${libc_SOURCE_DIR}/src/string/strncpy.c")
