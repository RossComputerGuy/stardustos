/**
	* StardustOS libc - (C) 2019 Tristan Ross
	*/
#include <string.h>

char* strncpy(char* dst, const char* src, size_t count) {
	char* ds = dst;
	char* ss = (char*)src;
	while (count > 0) {
		count--;
		if ((*ds++ = *ss++) == '\0') break;
	}
	while (count-- > 0) *ds++ = '\0';
	return dst;
}
