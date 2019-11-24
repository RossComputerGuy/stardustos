/**
 * NewLand Virtual Kernel - (C) 2019 Tristan Ross
 */
#define _GNU_SOURCE
#include <newland/errno.h>
#include <newland/fs.h>
#include <nvk/fs/rootfs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <liblist.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct rootfs {
	char* path;
	list_t cached_nodes;
};

struct rootfs_node {
	struct rootfs* rootfs;
	char* path;
};

static fs_node_t* rootfs_get_cached_node_byindex(fs_node_t* pnode, size_t index) {
	list_t cache;
	list_node_t* ln = NULL;
	if (!!strcmp(pnode->name, "/")) cache = ((struct rootfs_node*)pnode->impl)->rootfs->cached_nodes;
	else cache = ((struct rootfs*)pnode->impl)->cached_nodes;
	size_t i = 0;
	SLIST_FOREACH(ln, &cache, list) {
		fs_node_t* node = ln->value;
		struct rootfs_node* rootfs_node = (struct rootfs_node*)node->impl;
		if (i == index) return node;
		i++;
	}
	return NULL;
}

static size_t rootfs_write(fs_node_t* node, off_t offset, const void* buff, size_t size) {
	struct rootfs_node* rootfs_node = node->impl;
	int fd = open(rootfs_node->path, O_WRONLY);
	if (fd < 0) return -NEWLAND_EINVAL;
	lseek(fd, offset, SEEK_SET);
	size_t n = write(fd, buff, size);
	close(fd);
	return n;
}

static size_t rootfs_read(fs_node_t* node, off_t offset, void* buff, size_t size) {
	struct rootfs_node* rootfs_node = node->impl;
	int fd = open(rootfs_node->path, O_RDONLY);
	if (fd < 0) return -NEWLAND_EINVAL;
	lseek(fd, offset, SEEK_SET);
	size_t n = read(fd, buff, size);
	close(fd);
	return n;
}

static int rootfs_get_child(fs_node_t* node, fs_node_t** childptr, size_t index) {
	struct rootfs* rootfs = NULL;
	char* path = NULL;
	if (!!strcmp(node->name, "/")) {
		rootfs = ((struct rootfs_node*)node->impl)->rootfs;
		path = ((struct rootfs_node*)node->impl)->path;
	} else {
		rootfs = ((struct rootfs*)node->impl);
		path = rootfs->path;
	}
	fs_node_t* cached_node = rootfs_get_cached_node_byindex(node, index);
	if (cached_node == NULL) {
		index -= liblist_length(&rootfs->cached_nodes);
		DIR* dir = opendir(path);
		if (dir == NULL) return -NEWLAND_EINVAL;
		seekdir(dir, index);
		struct dirent* dirent = NULL;
		while ((dirent = readdir(dir)) != NULL) {
			if (!!strcmp(dirent->d_name, ".") && !!strcmp(dirent->d_name, "..")) {
				if (index == 0) {
					char* p;
					asprintf(&p, "%s/%s", path, dirent->d_name);
					struct stat st;
					if (stat(p, &st) < 0) {
						free(p);
						return -NEWLAND_ENOENT;
					}
					int type = 0;
					switch (st.st_mode & S_IFMT) {
						case S_IFDIR:
							type = FS_NODE_DIR;
							break;
						case S_IFREG:
							type = FS_NODE_FILE;
							break;
						default:
							free(p);
							return -NEWLAND_EINVAL;
					}
					struct rootfs_node* rootfs_node = malloc(sizeof(struct rootfs_node));
					fs_node_t* node;
					int r = fs_node_create(&node, dirent->d_name, 6 << type);
					if (r < 0) {
						free(p);
						free(rootfs_node);
						return r;
					}
					rootfs_node->rootfs = rootfs;
					rootfs_node->path = p;
					node->size = st.st_size;
					node->impl = rootfs_node;
					node->opts.get_child = rootfs_get_child;
					node->opts.read = rootfs_read;
					node->opts.write = rootfs_write;
					liblist_add(&rootfs->cached_nodes, node);
					*childptr = node;
					closedir(dir);
					return 0;
				}
				index--;
			}
		}
		closedir(dir);
		return -NEWLAND_ENOENT;
	} else *childptr = cached_node;
	return 0;
}

static int rootfs_mount(fs_node_t** targetptr, fs_node_t* source, unsigned long flags, const void* data) {
	struct rootfs* rootfs = malloc(sizeof(struct rootfs));
	if (rootfs == NULL) return -NEWLAND_ENOMEM;
	memset(rootfs, 0, sizeof(struct rootfs));
	rootfs->path = (char*)data;

	int r = fs_node_create(targetptr, "/", 6 << FS_NODE_DIR);
	if (r < 0) {
		free(rootfs);
		return r;
	}
	(*targetptr)->impl = rootfs;
	(*targetptr)->opts.get_child = rootfs_get_child;
	return 0;
}

static int rootfs_umount(fs_node_t** targetptr) {
	free(*targetptr);
	*targetptr = NULL;
	return 0;
}

static fs_opts_t rootfs_opts = { .mount = rootfs_mount, .umount = rootfs_umount };

int rootfs_init() {
	return register_fs("rootfs", FS_PSEUDO, rootfs_opts);
}