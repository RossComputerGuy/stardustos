/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#pragma once

#include <newland/fs.h>
#include <newland/list.h>
#include <newland/types.h>

#define DEV_MAJOR(dev) ((dev) >> 8)
#define DEV_MINOR(dev) ((dev) & 0xFF)
#define MKDEV(ma, mi) ((ma) << 8 | (mi))

#define DEVMAJ_BLOCK 1
#define DEVMAJ_TTY 2

typedef struct device {
  SLIST_ENTRY(struct device) dev_list;
  dev_t dev;
  const char name[NAME_MAX];
  fs_node_opts_t opts;
} device_t;

size_t device_count();
device_t* device_get(size_t i);
device_t* device_fromdev(dev_t dev);
device_t* device_fromname(const char* name);

int register_device(dev_t dev, const char* name, fs_node_opts_t opts);
int unregister_device(dev_t dev);