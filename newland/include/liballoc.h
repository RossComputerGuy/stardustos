#pragma once

#include <sys/types.h>
#include <malloc.h>
#include <string.h>

struct boundary_tag {
	unsigned int magic;
	unsigned int size;
	unsigned int real_size;
	int index;
	struct boundary_tag* split_left;
	struct boundary_tag* split_right;
	struct boundary_tag* next;
	struct boundary_tag* prev;
};

void* malloc(size_t);
void* realloc(void*, size_t);
void* calloc(size_t, size_t);
void free(void *);
