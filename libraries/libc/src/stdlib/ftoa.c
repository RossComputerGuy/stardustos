/**
	* StardustOS libc - (C) 2019 Tristan Ross
	*/
#include <sys/types.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

static int ntu(int num, int count) {
	int res = 1;
	while (count-- > 0) res *= num;
	return res;
}

int ftoa(float value, char* ptr) {
	if (ptr == NULL) return 0;
	int sign = -1;
	if (value < 0) {
		sign = '-';
		value = -value;
	}
	float num2 = value;
	long long int num = value;
	size_t len = 0;
	size_t len2 = 0;
	while ((num2 - (float)num) != 0.0 && !((num2 - (float)num) < 0.0)) {
		num2 = value * (ntu(10.0, len2 + 1));
		num = num2;
		len2++;
	}
	for (len = (value > 1) ? 0 : 1; value > 1; len++) value /= 10;
	size_t pos = len;
	len = len + 1 + len2;
	num = num2;
	if (sign == '-') {
		len++;
		pos++;
	}
	size_t wl = 0;
	for (int8_t i = len; i >= 0; i--) {
		if (i == len) ptr[i] = 0;
		else if (i == pos) ptr[i] = '.';
		else if (sign == '-' && i == 0) ptr[i] = '-';
		else {
			ptr[i] = (num % 10) + '0';
			num /= 10;
		}
		wl++;
	}
	return wl;
}
