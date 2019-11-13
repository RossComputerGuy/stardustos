/**
	* StardustOS libc - (C) 2019 Tristan Ross
	*/
#include <string.h>

int memcmp(const void *str1, const void *str2, size_t n) {
	unsigned char* p1 = (unsigned char*)str1;
	unsigned char* p2 = (unsigned char*)str2;
	while (n--) {
		if (*p1 != *p2) return *p1 - *p2;
		p1++;
		p2++;
	}
	return 0;
}