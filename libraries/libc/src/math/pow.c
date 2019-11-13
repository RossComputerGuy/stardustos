/**
	* StardustOS libc - (C) 2019 Tristan Ross
	*/
#include <math.h>

double pow(double x, double y) {
	double v = x;
	while (y-- > 0) v *= x;
	return v;
}