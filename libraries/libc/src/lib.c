/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#include <stdio.h>

extern int __file_write(FILE* stream, const void* buff, size_t size);
extern int __file_read(FILE* stream, void* buff, size_t size);

const FILE __libc_stdin = { .fd = 0, .impl = NULL, .mode = "r", .offset = 0, .write = __file_write, .read = __file_read };
const FILE __libc_stdout = { .fd = 1, .impl = NULL, .mode = "w", .offset = 0, .write = __file_write, .read = __file_read };
const FILE __libc_stderr = { .fd = 2, .impl = NULL, .mode = "w", .offset = 0, .write = __file_write, .read = __file_read };

int errno = 0;

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
