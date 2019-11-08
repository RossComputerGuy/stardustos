/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#pragma once

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifdef NEWLAND_KERNEL
#include <newland/alloc.h>

#define malloc kmalloc
#define realloc krealloc
#define free kfree
#endif

int ftoa(float value, char* ptr, int dig);
char* ultoa(unsigned long value, char* ptr, int base);