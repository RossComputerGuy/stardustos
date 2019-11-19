/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/dev/block.h>
#include <newland/alloc.h>
#include <newland/dev.h>
#include <newland/log.h>
#include <newland/errno.h>

SLIST_HEAD(blkdev_list, blkdev_t);
static struct blkdev_list block_devices = { NULL };
static size_t block_device_count = 0;

/** Finding **/
size_t blkdev_count() {
	return block_device_count;
}

size_t blkdev_indexof(blkdev_t* blkdev1) {
	blkdev_t* blkdev2 = NULL;
	size_t index = 0;
	SLIST_FOREACH(blkdev2, &block_devices, blkdev_list) {
		if (!strcmp(blkdev2->name, blkdev1->name)) return index;
		index++;
	}
	return -1;
}

blkdev_t* blkdev_get(size_t i) {
	blkdev_t* blkdev = NULL;
	size_t index = 0;
	SLIST_FOREACH(blkdev, &block_devices, blkdev_list) {
		if (index == i) return blkdev;
		index++;
	}
	return NULL;
}

blkdev_t* blkdev_fromname(const char* name) {
	blkdev_t* blkdev = NULL;
	SLIST_FOREACH(blkdev, &block_devices, blkdev_list) {
		if (!strcmp(blkdev->name, name)) return blkdev;
	}
	return NULL;
}

/** File Node Operations **/
static size_t blkdev_read(fs_node_t* node, off_t offset, void* buff, size_t size) {
	blkdev_t* blkdev = blkdev_get(DEV_MINOR(node->rdev));
	if (blkdev == NULL) return -NEWLAND_ENODEV;
	if (blkdev->opts.read_block == NULL) return -NEWLAND_ENOSYS;
	return blkdev->opts.read_block(blkdev, offset / blkdev->size, buff, size);
}

static size_t blkdev_write(fs_node_t* node, off_t offset, const void* buff, size_t size) {
	blkdev_t* blkdev = blkdev_get(DEV_MINOR(node->rdev));
	if (blkdev == NULL) return -NEWLAND_ENODEV;
	if (blkdev->opts.write_block == NULL) return -NEWLAND_ENOSYS;
	return blkdev->opts.write_block(blkdev, offset / blkdev->size, buff, size);
}

static int blkdev_ioctl(fs_node_t* node, int req, va_list ap) {
	blkdev_t* blkdev = blkdev_get(DEV_MINOR(node->rdev));
	if (blkdev == NULL) return -NEWLAND_ENODEV;
	switch (req) {
		case BLKDEV_SIZE:
			{
				blksize_t* ptr = va_arg(ap, blksize_t*);
				*ptr = blkdev->size;
			}
			return 0;
		case BLKDEV_COUNT:
			{
				blkcnt_t* ptr = va_arg(ap, blkcnt_t*);
				*ptr = blkdev->count;
			}
			return 0;
	}
	return -NEWLAND_EINVAL;
}

static fs_node_opts_t blkdev_fsopts = {
	.read = blkdev_read,
	.write = blkdev_write,
	.ioctl = blkdev_ioctl
};

/** Registration **/
int register_blkdev(const char* name, blksize_t blksize, blkcnt_t blkcnt, blkdev_opts_t opts) {
	if (blkdev_fromname(name) != NULL || device_fromname(name) != NULL) return -NEWLAND_EEXIST;
	blkdev_t* blkdev = kmalloc(sizeof(blkdev_t));
	if (blkdev == NULL) return -NEWLAND_ENOMEM;
	strcpy((char*)blkdev->name, name);
	blkdev->size = blksize;
	blkdev->count = blkcnt;
	blkdev->opts = opts;
	int r = register_device(MKDEV(DEVMAJ_BLOCK, block_device_count), name, blkdev_fsopts, blkdev->size * blkdev->count);
	if (r < 0) {
		kfree(blkdev);
		return r;
	}
	printk(KLOG_INFO "blkdev: registered %s", name);
	SLIST_INSERT_HEAD(&block_devices, blkdev, blkdev_list);
	block_device_count++;
	return 0;
}

int unregister_blkdev(const char* name) {
	blkdev_t* blkdev = blkdev_fromname(name);
	if (blkdev == NULL || device_fromname(name) == NULL) return -NEWLAND_ENODEV;
	size_t idx = blkdev_indexof(blkdev);
	int r = unregister_device(MKDEV(DEVMAJ_BLOCK, idx));
	if (r < 0) return r;
	printk(KLOG_INFO "blkdev: unregistered %s", name);
	SLIST_REMOVE(&block_devices, blkdev, blkdev_t, blkdev_list);
	block_device_count--;
	kfree(blkdev);
	return 0;
}
