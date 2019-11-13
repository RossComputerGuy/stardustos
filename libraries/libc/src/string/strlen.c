/**
	* StardustOS libc - (C) 2019 Tristan Ross
	*/
#include <string.h>

size_t strlen(const char* str) {
	size_t len = 0;
	while (*str++ != '\0') len++;
	return len;
}
