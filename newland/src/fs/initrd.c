/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/fs/initrd.h>
#include <newland/alloc.h>
#include <newland/fs.h>
#include <errno.h>
#include <string.h>

static int initrd_mount(fs_node_t** targetptr, fs_node_t* source, unsigned long flags, const void* data) {
  int r = fs_node_create(targetptr, "/", FS_NODE_DIR);
  if (r < 0) return r;
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
