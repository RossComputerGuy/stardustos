/**
 * NewLand Virtual Kernel - (C) 2019 Tristan Ross
 */
#include <time.h>

time_t boot_time = 0;

time_t boottime(time_t* t) {
	time_t v = time(NULL) - boot_time;
	if (t != NULL) *t = v;
	return v;
}
