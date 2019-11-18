/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/time.h>
#include <string.h>

static const int dpm[2][MONTH_PER_YEAR] = {
	{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

static const int dpy[2] = {365, 366};

extern int arch_clock(datetime_t* dt);

static time_t dt2t(datetime_t dt) {
	time_t t = 0;
	for (int year = EPOCH_YEAR; year < dt.yr; year++) t += dpy[IS_LEAP_YEAR(year)] * SECONDS_PER_DAY;
	for (int month = 0; month < dt.mon - 1; month++) t += dpm[IS_LEAP_YEAR(dt.yr)][month] * SECONDS_PER_DAY;
	t += (dt.day - 1) * SECONDS_PER_DAY;
	t += dt.hr * SECONDS_PER_HOURS;
	t += dt.min * SECONDS_PER_MINUTE;
	t += dt.sec;
	return t;
}

#ifndef __nvk__
time_t time(time_t* t) {
	datetime_t dt;
	arch_clock(&dt);
	time_t i = dt2t(dt);
	if (t != NULL) *t = i;
	return i;
}
#endif