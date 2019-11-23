/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#pragma once

#include <sys/types.h>
#include <string.h>

#define NL_PROP_ISNVK 1

int nl_getbuildprop(int prop, void* output, int* size);

pid_t nl_getpid();
uid_t nl_getuid();
gid_t nl_getgid();

int nl_open(const char* path, int mode);
int nl_close(int fd);
size_t nl_write(int fd, const void* buff, size_t count);
size_t nl_read(int fd, void* buff, size_t count);