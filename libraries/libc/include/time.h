/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#pragma once

#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1000000
#endif

typedef long time_t;
typedef long clock_t;

clock_t clock();
