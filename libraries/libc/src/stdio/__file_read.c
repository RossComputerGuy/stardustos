/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#ifdef NEWLAND_KERNEL
#include <newland/fs.h>
#endif
#include <stdio.h>

int __file_read(FILE* stream, void* buff, size_t size) {
#ifdef NEWLAND_KERNEL
	fs_node_t* node = stream->impl;
	return fs_node_read(&node, stream->offset, buff, size);
#else
	// TODO: system call
	return 0;
#endif
}
