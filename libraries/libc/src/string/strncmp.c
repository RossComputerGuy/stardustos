/**
	* StardustOS libc - (C) 2019 Tristan Ross
	*/
#include <string.h>

int strncmp(const char* s1, const char* s2, size_t n) {
	if (n == 0) return 0;
	while (n-- != 0 && *s1 == *s2) {
		if (n == 0 || *s1 == '\0') break;
		s1++;
		s2++;
	}
	return (*(unsigned char*)s1) - (*(unsigned char*)s2);
}
