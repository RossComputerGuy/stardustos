/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/arch/proc.h>
#include <newland/fs/procfs.h>
#include <newland/alloc.h>
#include <newland/fs.h>
#include <newland/errno.h>
#include <stdio.h>
#include <string.h>

static list_t procs = { NULL };

static void procfs_cleanup() {
	if (liblist_length(&procs) != process_count()) {
		for (size_t i = 0; i < liblist_length(&procs); i++) {
			fs_node_t* node = (fs_node_t*)liblist_get(&procs, i);
			proc_t* proc = process_get(i);
			char name[NAME_MAX];
			snprintf(name, NAME_MAX, "%lu", proc->id);
			if (node->impl == NULL || !!strcmp(name, node->name)) {
				liblist_remove(&procs, node);
				kfree(node);
				continue;
			}
		}
	}
}

static int procfs_get_child(fs_node_t* node, fs_node_t** childptr, size_t index) {
	procfs_cleanup();
	if (liblist_indexof(&procs, node) == -1) {
		// TODO: add /self symlink
		if (index >= process_count()) return -NEWLAND_ENOENT;
		if (liblist_get(&procs, index) == NULL) {
			char name[NAME_MAX];
			snprintf(name, NAME_MAX, "%lu", index);
			int r = fs_node_create(childptr, name, 6 << FS_NODE_DIR);
			if (r < 0) return r;
			(*childptr)->impl = process_get(index);
			r = liblist_add(&procs, *childptr);
			if (r < 0) {
				kfree(*childptr);
				return r;
			}
			return 0;
		} else {
		  *childptr = (fs_node_t*)liblist_get(&procs, index);
			return 0;
		}
	}
	return -NEWLAND_ENOENT;
}

static int procfs_mount(fs_node_t** targetptr, fs_node_t* source, unsigned long flags, const void* data) {
	int r = fs_node_create(targetptr, "/", 6 << FS_NODE_DIR);
	if (r < 0) return r;
	(*targetptr)->opts.get_child = procfs_get_child;
	return 0;
}

static int procfs_umount(fs_node_t** targetptr) {
	kfree(*targetptr);
	*targetptr = NULL;
	return 0;
}

static fs_opts_t procfs_opts = { .mount = procfs_mount, .umount = procfs_umount };

int procfs_init() {
	return register_fs("procfs", FS_PSEUDO, procfs_opts);
}
