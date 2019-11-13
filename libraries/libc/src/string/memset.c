/**
	* StardustOS libc - (C) 2019 Tristan Ross
	*/
#include <string.h>

void* memset(void* ptr, int val, size_t size) {
	unsigned char* buff = (unsigned char*)ptr;
	for (size_t i = 0; i < size; i++) buff[i] = val;
	return ptr;
}
