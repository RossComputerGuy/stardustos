/**
	* StardustOS libc - (C) 2019 Tristan Ross
	*/
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* convert(unsigned int num, int base) {
	static char* rep = "0123456789ABCDEF";
	static char buff[50];
	char* ptr = &buff[49];
	*ptr = '\0';
	do {
		*--ptr = rep[num % base];
		num /= base;
	} while (num != 0);
	return ptr;
}

int vsnprintf(char* str, size_t size, const char* format, va_list ap) {
	int wrote = 0;
	char* traverse = (char*)format;
	int i = 0;
	unsigned int si = 0;
	char* s = NULL;
	for (; *traverse != 0; traverse++) {
		while (*traverse != '%' && *traverse != 0) {
			if (wrote < size) str[wrote] = *traverse;
			wrote++;
			traverse++;
		}
		if (*traverse == 0) break;
		traverse++;
		switch (*traverse) {
			case 'c':
				i = va_arg(ap, int);
				if (wrote < size) str[wrote] = *traverse;
				wrote++;
				break;
			case 'd':
				i = va_arg(ap, int);
				if (i < 0) {
					i = -i;
					if (wrote < size) str[wrote] = '-';
					wrote++;
				}
				s = convert(i, 10);
				if (wrote < size) strncpy((char*)(str + wrote), s, strlen(s));
				wrote += strlen(s);
				break;
			case 'o':
				si = va_arg(ap, unsigned int);
				s = convert(si, 10);
				if (wrote < size) strncpy((char*)(str + wrote), s, strlen(s));
				wrote += strlen(s);
				break;
			case 's':
				s = va_arg(ap, char*);
				if (s == NULL) s = "(null)";
				if (wrote < size) strncpy((char*)(str + wrote), s, strlen(s));
				wrote += strlen(s);
				break;
			case 'x':
				si = va_arg(ap, unsigned int);
				s = convert(si, 10);
				if (wrote < size) strncpy((char*)(str + wrote), s, strlen(s));
				wrote += strlen(s);
				break;
			case 'f':
				{
					double v = va_arg(ap, double);
					static char temp[50];
					i = ftoa(v, temp);
					if (wrote < size) strncpy((char*)(str + wrote), temp, i);
					wrote += i;
				}
				break;
			case 'l':
				if (traverse[1] == 'u') {
					unsigned long lu = va_arg(ap, unsigned long);
					s = convert(lu, 10);
				} else {
					long l = va_arg(ap, long);
					s = convert(l, 10);
				}
				if (wrote < size) strncpy((char*)(str + wrote), s, strlen(s));
				wrote += strlen(s);
				break;
			case '%':
				if (wrote < size) str[wrote] = '%';
				wrote++;
				break;
		}
	}
	str[wrote] = 0;
	return wrote - 1;
}
