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
  void* buff;
};

static int initrd_mount(fs_node_t** targetptr, fs_node_t* source, unsigned long flags, const void* data) {
/** Decompress **/
  struct initrd* initrd = kmalloc(sizeof(struct initrd));
  if (initrd == NULL) return -ENOMEM;

  if ((initrd->buff = kmalloc(source->size)) == NULL) {
    kfree(initrd);
    return -ENOMEM;
  }

  int r = fs_node_read(&source, 0, initrd->buff, source->size);
  if (r < 0) {
    kfree(initrd->buff);
    kfree(initrd);
    return r;
  }

  if (!mz_zip_reader_init_mem(&initrd->zip, initrd->buff, source->size, MZ_ZIP_FLAG_CASE_SENSITIVE | MZ_ZIP_FLAG_COMPRESSED_DATA)) {
    kfree(initrd->buff);
    kfree(initrd);
    printk(KLOG_ERR "initrd: miniz failed to initialize zip reader: %s\n", mz_zip_get_error_string(mz_zip_get_last_error(&initrd->zip)));
    return -EINVAL;
  }

/** Root node creation **/
  r = fs_node_create(targetptr, "/", 6 << FS_NODE_DIR);
  if (r < 0) return r;
  (*targetptr)->dev = source->rdev;
  (*targetptr)->impl = initrd;
  // TODO: implement the get children function
  return 0;
}

static int initrd_umount(fs_node_t** targetptr) {
  struct initrd* initrd = (struct initrd*)((*targetptr)->impl);
  // TODO: free nodes
  kfree(initrd->buff);
  mz_zip_reader_end(&initrd->zip);
  kfree(initrd);
  kfree(*targetptr);
  *targetptr = NULL;
  return 0;
}

static fs_opts_t initrd_opts = { .mount = initrd_mount, .umount = initrd_umount };

int initrd_init() {
  return register_fs("initrd", FS_VIRT, initrd_opts);
}
