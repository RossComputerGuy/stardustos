/**
	* StardustOS libc - (C) 2019 Tristan Ross
	*/
#include <stdlib.h>

char* ultoa(unsigned long value, char* ptr, int base) {
	unsigned long t = 0;
	unsigned long tmp = value;
	int count = 0;
	if (ptr == NULL) return NULL;
	if (tmp == 0) count++;
	while (tmp > 0) {
		tmp = tmp/base;
		count++;
	}
	ptr += count;
	*ptr = '\0';
	do {
		unsigned long res = value - base * (t = value / base);
		if (res < 10) * -- ptr = '0' + res;
		else if ((res >= 10) && (res < 16)) * --ptr = 'A' - 10 + res;
	} while ((value = t) != 0);
	return ptr;
}
