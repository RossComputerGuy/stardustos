/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#pragma once

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef __TYPES_size_t
#define __TYPES_size_t
typedef unsigned int size_t;
#endif

#ifndef max
#define max(a, b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#endif

#ifndef min
#define min(a, b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#endif

#ifdef NEWLAND_KERNEL
#include <newland/alloc.h>

#define malloc kmalloc
#define realloc krealloc
#define free kfree
#else
void* malloc(size_t size);
void* realloc(void* ptr, size_t size);
void free(void* ptr);
#endif

int ftoa(float value, char* ptr);
char* ultoa(unsigned long value, char* ptr, int base);
