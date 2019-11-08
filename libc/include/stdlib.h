/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#pragma once

#ifndef NULL
#define NULL ((void*)0)
#endif

int ftoa(float value, char* ptr, int dig);
char* ultoa(unsigned long value, char* ptr, int base);