/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#include <stdio.h>

// TODO: set the write and read functions
const FILE __libc_stdin = { .fd = 0, .impl = NULL, .mode = "r", .offset = 0 };
const FILE __libc_stdout = { .fd = 1, .impl = NULL, .mode = "w", .offset = 0 };
const FILE __libc_stderr = { .fd = 2, .impl = NULL, .mode = "w", .offset = 0 };

#ifndef NEWLAND_KERNEL
extern void _init();
extern void _fini();

void __libc_init() {
  // TODO: open stdin, stdout, and stderr
  _init();
}

void __libc_fini(int code) {
  _fini();
  // TODO: close stdin, stdout, and stderr
  while (1);
}
#endif
