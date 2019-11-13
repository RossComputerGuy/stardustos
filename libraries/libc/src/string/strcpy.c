/**
	* StardustOS libc - (C) 2019 Tristan Ross
	*/
#include <string.h>

char* strcpy(char* dst, const char* src) {
	char* s = dst;
	while (*dst++ = *src++);
	return s;
}
