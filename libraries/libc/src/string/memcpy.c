/**
	* StardustOS libc - (C) 2019 Tristan Ross
	*/
#include <string.h>

void* memcpy(void* dst, const void* src, size_t size) {
	unsigned char* buffdst = (unsigned char*)dst;
	const unsigned char* buffsrc = (const unsigned char*)src;
	for (size_t i = 0; i < size; i++) buffdst[i] = buffsrc[i];
	return buffdst;
}
