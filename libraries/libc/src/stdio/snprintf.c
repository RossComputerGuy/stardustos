/**
	* StardustOS libc - (C) 2019 Tristan Ross
	*/
#include <stdarg.h>
#include <stdio.h>

int snprintf(char* str, size_t size, const char* format, ...) {
	va_list ap;
	va_start(ap, format);
	int r = vsnprintf(str, size, format, ap);
	va_end(ap);
	return r;
}