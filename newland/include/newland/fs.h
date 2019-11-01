/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#pragma once

#include <newland/limits.h>
#include <newland/list.h>
#include <newland/time.h>
#include <newland/types.h>
#include <stdarg.h>
#include <string.h>

#define FS_NODE_FILE 1
#define FS_NODE_DIR 2
#define FS_NODE_SOCK 3
#define FS_NODE_LINK 4
#define FS_NODE_BLK 5
#define FS_NODE_CHR 6
#define FS_NODE_TYPE_SET(mode, type) (mode |= (6 << type))
#define FS_NODE_TYPE_GET(mode) (mode >> 6)

struct fs_node;

typedef struct {
  struct fs_node* node;
  gid_t gid;
  uid_t uid;
  pid_t pid;
  off_t offset;
} fd_t;

typedef struct {
  int (*open)(struct fs_node* node, fd_t* fd);
  int (*close)(struct fs_node* node, fd_t* fd);
  int (*get_child)(struct fs_node* node, struct fs_node** childptr, size_t index);
  int (*mknode)(struct fs_node* node, struct fs_node** childptr, const char* name, mode_t mode);
  int (*rmnode)(struct fs_node* node, struct fs_node** childptr);
  size_t (*read)(struct fs_node* node, off_t offset, void* buff, size_t size);
  size_t (*write)(struct fs_node* node, off_t offset, const void* buff, size_t size);
  int (*ioctl)(struct fs_node* node, int req, va_list ap);
} fs_node_opts_t;

typedef struct fs_node {
  const char name[NAME_MAX];
  dev_t dev;
  ino_t ino;
  mode_t mode;
  nlink_t nlink;
  uid_t uid;
  gid_t gid;
  dev_t rdev;
  size_t size;
  blksize_t blksize;
  blkcnt_t blkcnt;
  time_t atime;
  time_t mtime;
  time_t ctime;
  fs_node_opts_t opts;
  void* impl;
} fs_node_t;

#define FS_PSEUDO 0
#define FS_VIRT 1
#define FS_PHYS 2

typedef struct fs {
  SLIST_ENTRY(struct fs) fs_list;
  int (*mount)(fs_node_t** targetptr, fs_node_t* source, unsigned long flags, const void* data);
  int (*umount)(fs_node_t** targetptr);
  const char name[NAME_MAX];
  int type;
} fs_t;

#define MS_BIND (1 << 0)

typedef struct mountpoint {
  SLIST_ENTRY(struct mountpoint) mp_list;
  fs_node_t* rootnode;
  const char fsname[NAME_MAX];
  const char source[PATH_MAX];
  const char target[PATH_MAX];
  unsigned long flags;
} mountpoint_t;

int fs_node_create(fs_node_t** nodeptr, const char* name, mode_t mode);
size_t fs_node_read(fs_node_t** nodeptr, off_t offset, void* buff, size_t size);
size_t fs_node_write(fs_node_t** nodeptr, off_t offset, const void* buff, size_t size);
int fs_node_vioctl(fs_node_t** nodeptr, int req, va_list ap);
int fs_node_ioctl(fs_node_t** nodeptr, int req, ...);

size_t mountpoint_count();
mountpoint_t* mountpoint_fromsrc(const char* src);
mountpoint_t* mountpoint_fromtarget(const char* target);
int mountpoint_create(fs_t** fsptr, const char* src, const char* target, unsigned long flags, const void* data);