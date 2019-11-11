/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#pragma once

#include <stdarg.h>
#include <stddef.h>

#ifndef stdin
#define stdin ((FILE*)0)
#endif

#ifndef stdout
#define stdout ((FILE*)1)
#endif

#ifndef stderr
#define stderr ((FILE*)2)
#endif

typedef int FILE;

int fprintf(FILE* stream, const char* format, ...);

void perror(const char *s);

int printf(const char* format, ...);
int putc(const char c, FILE* stream);

int snprintf(char* str, size_t size, const char* format, ...);
int vsnprintf(char* str, size_t size, const char* format, va_list ap);
