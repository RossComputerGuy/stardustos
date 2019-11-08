/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#pragma once

#include <stdarg.h>

#ifndef __TYPE_size_t
#define __TYPE_size_t
typedef unsigned int size_t;
#endif

int snprintf(char* str, size_t size, const char* format, ...);
int vsnprintf(char* str, size_t size, const char* format, va_list ap);