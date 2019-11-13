/**
	* StardustOS libc - (C) 2019 Tristan Ross
	*/
#include <string.h>

void* memmove(void* dst, const void* src, size_t size) {
	unsigned char* dstbuff = (unsigned char*)dst;
	const unsigned char* srcbuff = (const unsigned char*)src;
	if (dstbuff < srcbuff) {
		for (size_t i = 0; i < size; i++) dstbuff[i] = srcbuff[i];
	} else {
		for (size_t i = size; i != 0; i--) dstbuff[i-1] = srcbuff[i-1];
	}
	return dstbuff;
}
