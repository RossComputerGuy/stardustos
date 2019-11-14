/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#pragma once

#include <newland/fs.h>

fs_node_t* devfs_get(size_t i);
int devfs_init();
