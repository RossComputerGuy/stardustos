/**
  * StardustOS libfile - (C) 2019 Tristan Ross
  */
#include <libfile/tar.h>
#include <errno.h>

struct rawblock {
  char name[100];
  char mode[8];
  char uid[8];
  char gid[8];
  char size[12];
  char mtime[12];
  char chksum[8];
  char typeflag;
  char linkname[100];
  char magic[6];
  char version[2];
  char uname[32];
  char gname[32];
  char devmajor[8];
  char devminor[8];
  char prefix[155];
};

static size_t getfilesize(struct rawblock* block) {
  size_t size = 0;
  size_t count = 0;
  for (size_t i = 11; i > 0; i--, count *= 8) size += ((block->size[i - 1] - '0') * count);
  return size;
}

size_t tar_getcount(void* buff) {
  struct rawblock* block = (struct rawblock*)buff;
  size_t count = 0;
  while (block->name[0] != 0) {
    count++;
    size_t size = getfilesize(block);
    block = (struct rawblock*)(block + ((size / 512) + 1) * 512);
    if (size % 512)  block = (struct rawblock*)(block + 512);
  }
  return count;
}

int tar_read(void* buff, tar_blk_t* blk, size_t index) {
  struct rawblock* block = (struct rawblock*)buff;
  for (size_t i = 0; i < index; i++) {
    if (block->name[0] == 0) return -EINVAL;
    size_t size = getfilesize(block);
    block = (struct rawblock*)(block + ((size / 512) + 1) * 512);
    if (size % 512)  block = (struct rawblock*)(block + 512);
  }
  if (block->name[0] == 0) return -EINVAL;
  memcpy(blk->name, block->name, 100);
  blk->size = getfilesize(block);
  blk->data = (void*)(block + 512);
  return 0;
}