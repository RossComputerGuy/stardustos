set(ARCH_FILES "${ARCH_DIR}/src/asm/entry.s"
  "${ARCH_DIR}/src/bootstrap.c")
include_directories("${ARCH_DIR}/include")
add_link_options("-T${ARCH_DIR}/linker.ld" "-m32")
add_compile_options("-m32")