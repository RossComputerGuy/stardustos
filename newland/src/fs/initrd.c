/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/dev/block.h>
#include <newland/fs/initrd.h>
#include <newland/alloc.h>
#include <newland/dev.h>
#include <newland/fs.h>
#include <newland/log.h>
#include <errno.h>
#include <liblist.h>
#include <miniz.h>
#include <string.h>

struct initrd {
  mz_zip_archive zip;
  list_t nodes;
};

static int initrd_mount(fs_node_t** targetptr, fs_node_t* source, unsigned long flags, const void* data) {
/** Find device **/
  device_t* dev = device_fromdev(source->rdev);
  if (dev == NULL) return -ENODEV;
  blkdev_t* blkdev = blkdev_fromname(dev->name);
  if (blkdev == NULL) return -ENODEV;

/** Allocate buffer **/
  void* buff = kmalloc(blkdev->size);
  if (buff == NULL) return -ENOMEM;

/** Decompress **/
  struct initrd* initrd = kmalloc(sizeof(struct initrd));
  if (initrd == NULL) {
    kfree(buff);
    return -ENOMEM;
  }
  if (!mz_zip_reader_init(&initrd->zip, blkdev->size * blkdev->count, MZ_ZIP_FLAG_CASE_SENSITIVE | MZ_ZIP_FLAG_COMPRESSED_DATA)) {
    printk(KLOG_ERR "initrd: miniz failed to initialize zip reader: %s\n", mz_zip_get_error_string(mz_zip_get_last_error(&initrd->zip)));
    return -EINVAL;
  }

/** Root node creation **/
  int r = fs_node_create(targetptr, "/", FS_NODE_DIR);
  if (r < 0) return r;
  (*targetptr)->dev = dev->dev;
  (*targetptr)->impl = initrd;
  // TODO: implement the get children function
  return 0;
}

static int initrd_umount(fs_node_t** targetptr) {
  kfree(*targetptr);
  *targetptr = NULL;
  return 0;
}

static fs_opts_t initrd_opts = { .mount = initrd_mount, .umount = initrd_umount };

int initrd_init() {
  return register_fs("initrd", FS_VIRT, initrd_opts);
}
