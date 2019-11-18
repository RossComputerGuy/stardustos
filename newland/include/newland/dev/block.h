/** \file include/newland/dev/block.h
 * \author Tristan Ross
 * \copyright GNU Public License 3.0
 * \brief Block Device API
 * \since v0.1.0
 */
#pragma once

#include <sys/types.h>
#include <liblist.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

struct blkdev;

/**
 * Block device ioctl size request
 */
#define BLKDEV_SIZE 1

/**
 * Block device ioctl count request
 */
#define BLKDEV_COUNT 2

/**
 * Block Device Operations
 */
typedef struct {
	/**
	 * Read block function
	 *
	 * @param[in] blkdev The block device
	 * @param[in] block The block index to read
	 * @param[in] buff Buffer to store the read block
	 * @param[in] size The number of bytes to read
	 * @return The number of bytes read
	 */
	size_t (*read_block)(struct blkdev* blkdev, uint16_t block, void* buff, size_t size);

	/**
	 * Write block function
	 *
	 * @param[in] blkdev The block device
	 * @param[in] block The block index to read
	 * @param[in] buff Buffer to write
	 * @param[in] size The number of bytes to white
	 * @return The number of bytes written
	 */
	size_t (*write_block)(struct blkdev* blkdev, uint16_t block, const void* buff, size_t size);
} blkdev_opts_t;

/**
 * Block Device
 */
typedef struct blkdev {
	SLIST_ENTRY(struct blkdev) blkdev_list;

	/**
	 * The size of the blocks in bytes
	 */
	blksize_t size;

	/**
	 * The number of blocks
	 */
	blkcnt_t count;

	/**
	 * The name of the block device
	 */
	const char name[NAME_MAX];

	/**
	 * Block device operations
	 */
	blkdev_opts_t opts;
} blkdev_t;

/**
 * Get the number of registered block devices
 *
 * @return A number
 */
size_t blkdev_count();

/**
 * Gets the index of the block device
 *
 * @param[in] blkdev1 The block device
 * @return The index of the block device or -1 for not found
 */
size_t blkdev_indexof(blkdev_t* blkdev1);

/**
 * Gets a block device by its index
 *
 * @param[in] i The index of the block device
 * @return A block device or NULL if not found
 */
blkdev_t* blkdev_get(size_t i);

/**
 * Gets a block device by its name
 *
 * @param[in] name The name of the block device
 * @return A block device or NULL if not found
 */
blkdev_t* blkdev_fromname(const char* name);

/**
 * Registers a block device
 *
 * @param[in] name The name of the block device
 * @param[in] blksize The size of the blocks in bytes
 * @param[in] blkcnt The number of blocks
 * @param[in] opts The operations for the block device
 * @return Zero on success or a negative errno code
 */
int register_blkdev(const char* name, blksize_t blksize, blkcnt_t blkcnt, blkdev_opts_t opts);

/**
 * Unregisters a block device
 *
 * @param[in] name The name of the block device
 * @return Zero on success or a negative errno code
 */
int unregister_blkdev(const char* name);