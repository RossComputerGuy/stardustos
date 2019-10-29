/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#pragma once

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef __TYPE_size_t
#define __TYPE_size_t
typedef unsigned short size_t;
#endif

size_t strlen(const char* str);
void* memset(void* ptr, int val, size_t size);