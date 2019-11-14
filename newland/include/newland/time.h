/** \file include/newland/time.h
 * \author Tristan Ross
 * \copyright GNU Public License 3.0
 * \brief Kernel Time API
 * \since v0.1.0
 */
#pragma once

#include <time.h>

/**
 * Unix epoch year
 */
#define EPOCH_YEAR 1970

/**
 * Seconds per minute
 */
#define SECONDS_PER_MINUTE (60)

/**
 * Seconds per hours
 */
#define SECONDS_PER_HOURS (SECONDS_PER_MINUTE * 60)

/**
 * Seconds per day
 */
#define SECONDS_PER_DAY (SECONDS_PER_HOURS * 24)

/**
 * Months per year
 */
#define MONTH_PER_YEAR 12

/**
 * Gets a boolean value for if the year is a leap year
 *
 * @param[in] __year The year to check
 * @return A boolean value
 */
#define IS_LEAP_YEAR(__year) ((!((__year) % 4) && (__year) % 100) || !((__year) % 400))

/**
 * Structure to store the date and time
 */
typedef struct {
	int sec;
	int min;
	int hr;
	int day;
	int mon;
	int yr;
} datetime_t;

/**
 * Similar to the libc time function but returns the boot time
 *
 * @param[out] t Pointer to store the time the system has been up
 * @return The length of time the system has been up
 */
time_t boottime(time_t* t);
