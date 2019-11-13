/**
	* StardustOS libc - (C) 2019 Tristan Ross
	*/
#include <string.h>

int strcmp(const char* s1, const char* s2) {
	while (*s1 != '\0' && *s1 == *s2) {
		s1++;
		s2++;
	}
	return (*(unsigned char*)s1) - (*(unsigned char*)s2);
}
