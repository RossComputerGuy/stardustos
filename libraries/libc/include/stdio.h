/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#pragma once

#include <stdarg.h>
#include <stddef.h>

typedef struct FILE {
	int fd;
	void* impl;
	const char* mode;
	size_t offset;
	int (*write)(struct FILE* stream, const void* buff, size_t size);
	int (*read)(struct FILE* stream, void* buff, size_t size);
} FILE;

#ifndef stdin
extern const FILE __libc_stdin;
#define stdin ((FILE*)&__libc_stdin)
#endif

#ifndef stdout
extern const FILE __libc_stdout;
#define stdout ((FILE*)&__libc_stdout)
#endif

#ifndef stderr
extern const FILE __libc_stderr;
#define stderr ((FILE*)&__libc_stderr)
#endif

int fprintf(FILE* stream, const char* format, ...);

int printf(const char* format, ...);

int snprintf(char* str, size_t size, const char* format, ...);
int vfprintf(FILE* stream, const char* format, va_list ap);
int vsnprintf(char* str, size_t size, const char* format, va_list ap);
