/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#include <stdarg.h>
#include <stdio.h>

int fprintf(FILE* stream, const char* format, ...) {
	va_list ap;
	va_start(ap, format);
	int r = vfprintf(stream, format, ap);
	va_end(ap);
	return r;
}
