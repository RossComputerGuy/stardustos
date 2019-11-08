/**
  * StardustOS libfile - (C) 2019 Tristan Ross
  */
#pragma once

#include <sys/types.h>
#include <string.h>

typedef struct {
  char name[100];
  uint32_t size;
  void* data;
} tar_blk_t;

size_t tar_getcount(void* buff);
int tar_read(void* buff, tar_blk_t* blk, size_t index);