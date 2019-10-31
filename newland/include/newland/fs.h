/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#pragma once

#include <newland/limits.h>
#include <newland/types.h>
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

typedef struct fs_node {
  int (*open)(struct fs_node* node, fd_t* fd);
  int (*close)(struct fs_node* node, fd_t* fd);
  int (*get_child)(struct fs_node* node, struct fs_node** childptr, size_t index);
  int (*mknode)(struct fs_node* node, struct fs_node** childptr, const char* name, mode_t mode);
  int (*rmnode)(struct fs_node* node, struct fs_node** childptr);
} fs_node_opts_t;

typedef struct {
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
  // TODO: add time stuff
  fs_node_opts_t opts;
  void* impl;
} fs_node_t;

#define FS_PSEUDO 0
#define FS_VIRT 1
#define FS_PHYS 2

typedef struct {
  int (*mount)(fs_node_t** targetptr, fs_node_t* source, unsigned long flags, const void* data);
  int (*umount)(fs_node_t** targetptr);
  const char name[NAME_MAX];
  int type;
} fs_t;