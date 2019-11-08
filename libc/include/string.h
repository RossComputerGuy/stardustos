/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#pragma once

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef __TYPE_size_t
#define __TYPE_size_t
typedef unsigned int size_t;
#endif

void itoa(char* buf, int base, int d);

void* memcpy(void* dst, const void* src, size_t size);
void* memmove(void* dst, const void* src, size_t size);
void* memset(void* ptr, int val, size_t size);

int strcmp(const char* s1, const char* s2);
char* strcpy(char* dst, const char* src);
size_t strlen(const char* str);
int strncmp(const char* s1, const char* s2, size_t n);
char* strncpy(char* dst, const char* src, size_t count);
