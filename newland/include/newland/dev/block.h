/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#pragma once

#include <newland/limits.h>
#include <newland/types.h>
#include <liblist.h>
#include <string.h>

struct blkdev;

#define BLKDEV_SIZE 1
#define BLKDEV_COUNT 2

typedef struct {
	size_t (*read_block)(struct blkdev* blkdev, uint16_t block, void* buff, size_t size);
	size_t (*write_block)(struct blkdev* blkdev, uint16_t block, const void* buff, size_t size);
} blkdev_opts_t;

typedef struct blkdev {
	SLIST_ENTRY(struct blkdev) blkdev_list;
	blksize_t size;
	blkcnt_t count;
	const char name[NAME_MAX];
	blkdev_opts_t opts;
} blkdev_t;

size_t blkdev_count();
size_t blkdev_indexof(blkdev_t* blkdev1);
blkdev_t* blkdev_get(size_t i);
blkdev_t* blkdev_fromname(const char* name);

int register_blkdev(const char* name, blksize_t blksize, blkcnt_t blkcnt, blkdev_opts_t opts);
int unregister_blkdev(const char* name);
