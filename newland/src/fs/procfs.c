/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/arch/proc.h>
#include <newland/fs/procfs.h>
#include <newland/alloc.h>
#include <newland/fs.h>
#include <errno.h>
#include <string.h>

static list_t procs = { NULL };

static void procfs_cleanup() {
	if (list_length(&procs) != process_count()) {
		for (size_t i = 0; i < list_length(&procs); i++) {
			fs_node_t* node = (fs_node_t*)list_get(&procs, i);
			proc_t* proc = process_get(i);
			char name[NAME_MAX];
			itoa(name, 10, proc->id);
			if (node->impl == NULL || !!strcmp(name, node->name)) {
				list_remove(&procs, node);
				kfree(node);
				continue;
			}
		}
	}
}

static int procfs_get_child(fs_node_t* node, fs_node_t** childptr, size_t index) {
	procfs_cleanup();
	if (list_indexof(&procs, node) == -1) {
		// TODO: add /self symlink
		if (index >= process_count()) return -ENOENT;
		if (list_get(&procs, index) == NULL) {
			char name[NAME_MAX];
			itoa(name, 10, index);
			int r = fs_node_create(childptr, name, 6 << FS_NODE_DIR);
			if (r < 0) return r;
			(*childptr)->impl = process_get(index);
			r = list_add(&procs, *childptr);
			if (r < 0) {
				kfree(*childptr);
				return r;
			}
			return 0;
		} else {
		  *childptr = (fs_node_t*)list_get(&procs, index);
			return 0;
		}
	}
	return -ENOENT;
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
