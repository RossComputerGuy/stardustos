/**
	* StardustOS libc - (C) 2019 Tristan Ross
	*/
#include <string.h>

int itoa(char* buf, int base, int d) {
	char* p = buf;
	char* p1, *p2;
	unsigned long ud = d;
	int divisor = 10;
	int wrote = 0;

	/* If %d is specified and D is minus, put `-' in the head.	*/
	if (base == 'd' && d < 0) {
		*p++ = '-';
		buf++;
		wrote++;
		ud = -d;
	} else if (base == 'x') divisor = 16;

	/* Divide UD by DIVISOR until UD == 0.	*/
	do {
		int remainder = ud % divisor;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
	} while (ud /= divisor);

	/* Terminate BUF.	*/
	*p = 0;

	/* Reverse BUF.	*/
	p1 = buf;
	p2 = p - 1;
	while (p1 < p2) {
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
		wrote++;
	}
	return wrote;
}
