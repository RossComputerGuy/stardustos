/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#pragma once

#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1000000
#endif

typedef long time_t;
typedef long clock_t;

struct tm {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

clock_t clock();
time_t time(time_t* t);