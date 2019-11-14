/**
 * \copyright NewLand Kernel - (C) 2019 Tristan Ross
 */
#pragma once

#include <newland/arch/clock.h>
#include <time.h>

#define EPOCH_YEAR 1970
#define SECONDS_PER_MINUTE (60)
#define SECONDS_PER_HOURS (SECONDS_PER_MINUTE * 60)
#define SECONDS_PER_DAY (SECONDS_PER_HOURS * 24)
#define MONTH_PER_YEAR 12
#define IS_LEAP_YEAR(__year) ((!((__year) % 4) && (__year) % 100) || !((__year) % 400))

typedef struct {
	int sec;
	int min;
	int hr;
	int day;
	int mon;
	int yr;
} datetime_t;

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

time_t time(time_t* t);
time_t boottime(time_t* t);
