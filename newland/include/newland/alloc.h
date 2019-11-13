/**
	* NewLand Kernel - (C) 2019 Tristan Ross
	*/
#pragma once

#include <string.h>

void* kmalloc(size_t size);
void kfree(void* ptr);
void* krealloc(void* ptr, size_t size);
