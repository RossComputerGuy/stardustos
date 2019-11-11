/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#pragma once

#include <string.h>

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

size_t malloc_usable_size(void* ptr);
