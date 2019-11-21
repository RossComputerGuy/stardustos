/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/dev/block.h>
#include <newland/fs/devfs.h>
#include <newland/alloc.h>
#include <newland/dev.h>
#include <newland/fs.h>
#include <newland/errno.h>
#include <liblist.h>

static list_t devfs_nodes = { NULL };

fs_node_t* devfs_get(size_t i) {
	fs_node_t* node = liblist_get(&devfs_nodes, i);
	if (node == NULL) {
		device_t* device = device_get(i);
		if (device == NULL) return NULL;
		blkdev_t* blkdev = blkdev_fromname(device->name);
		int r = 0;
		if (blkdev != NULL) r = fs_node_create(&node, device->name, 6 << FS_NODE_BLK);
		else r = fs_node_create(&node, device->name , 6 << FS_NODE_CHR);
		if (r < 0) return NULL;
		if (blkdev != NULL) {
			node->blksize = blkdev->size;
			node->blkcnt = blkdev->count;
		}
		node->rdev = device->dev;
		node->size = device->size;
		node->opts = device->opts;
		liblist_add(&devfs_nodes, node);
		return node;
	}
	return node;
}

int devfs_init() {
	return 0;
}
