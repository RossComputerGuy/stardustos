/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#include <stdio.h>

int printf(const char* format, ...) {
	va_list ap;
	va_start(ap, format);
	int r = vfprintf(stdout, format, ap);
	va_end(ap);
	return r;
}
