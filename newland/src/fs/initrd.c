/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/dev/block.h>
#include <newland/fs/initrd.h>
#include <newland/alloc.h>
#include <newland/dev.h>
#include <newland/fs.h>
#include <newland/log.h>
#include <newland/errno.h>
#include <libgen.h>
#include <liblist.h>
#include <miniz.h>
#include <string.h>

struct initrd {
	mz_zip_archive zip;
	list_t nodes;
};

struct initrd_node {
	struct initrd* initrd;
	uint32_t index;
	char path[PATH_MAX];
};

/** Miniz functions **/

static size_t mz_initrd_read(void* opaque, mz_uint64 off, void* buff, size_t size) {
	fs_node_t* node = (fs_node_t*)opaque;
	return fs_node_read(&node, off, buff, size);
}

/** Filesystem operations **/
static size_t initrd_read(fs_node_t* node, off_t offset, void* buff, size_t size) {
	struct initrd_node* initrd_node = node->impl;
	struct initrd* initrd = initrd_node->initrd;
	size_t sz;
	void* _buff = mz_zip_reader_extract_to_heap(&initrd->zip, initrd_node->index, &sz, 0);
	if (_buff == NULL) return -NEWLAND_EINVAL;
	memcpy(buff, (void*)(((char*)_buff) + offset), size);
	kfree(_buff);
	return sz;
}

static int initrd_get_child(fs_node_t* node, fs_node_t** childptr, size_t index) {
	list_node_t* ln = NULL;
	struct initrd* initrd = NULL;
	char* path = NULL;
	if (!!strcmp(node->name, "/")) {
		initrd = ((struct initrd_node*)node->impl)->initrd;
		path = ((struct initrd_node*)node->impl)->path;
	} else {
		initrd = (struct initrd*)node->impl;
		path = "/";
	}
	SLIST_FOREACH(ln, &initrd->nodes, list) {
		char* d = dirname(path);
		d++;
		fs_node_t* n = ln->value;
		struct initrd_node* initrd_node = node->impl;
		mz_zip_archive_file_stat stat;
		if (!mz_zip_reader_file_stat(&initrd->zip, initrd_node->index, &stat)) continue;
		if (!strncmp(stat.m_filename, d, strlen(d))) {
			if (index == 0) {
				*childptr = n;
				return 0;
			}
			index--;
		}
	}
	return -NEWLAND_ENOENT;
}

static int initrd_mount(fs_node_t** targetptr, fs_node_t* source, unsigned long flags, const void* data) {
/** Decompress **/
	struct initrd* initrd = kmalloc(sizeof(struct initrd));
	if (initrd == NULL) return -NEWLAND_ENOMEM;

	initrd->zip.m_pRead = mz_initrd_read;
	initrd->zip.m_pIO_opaque = source;

	if (!mz_zip_reader_init(&initrd->zip, source->size, MZ_ZIP_FLAG_CASE_SENSITIVE | MZ_ZIP_FLAG_COMPRESSED_DATA)) {
		kfree(initrd);
		printk(KLOG_ERR "initrd: miniz failed to initialize zip reader: %s\n", mz_zip_get_error_string(mz_zip_get_last_error(&initrd->zip)));
		return -NEWLAND_EINVAL;
	}

/** Root node creation **/
	int r = fs_node_create(targetptr, "/", 6 << FS_NODE_DIR);
	if (r < 0) {
		kfree(initrd);
		return r;
	}
	(*targetptr)->dev = source->rdev;
	(*targetptr)->impl = initrd;
	(*targetptr)->opts.get_child = initrd_get_child;

	for (size_t i = 0; i < initrd->zip.m_total_files; i++) {
		fs_node_t* node;
		struct initrd_node* initrd_node = kmalloc(sizeof(struct initrd_node));
		if (initrd_node == NULL) {
			kfree(initrd);
			kfree((*targetptr));
			return -NEWLAND_ENOMEM;
		}
		mz_zip_archive_file_stat stat;
		if (!mz_zip_reader_file_stat(&initrd->zip, i, &stat)) {
			kfree(initrd);
			kfree((*targetptr));
			kfree(initrd_node);
			printk(KLOG_ERR "initrd: miniz failed to stat: %s\n", mz_zip_get_error_string(mz_zip_get_last_error(&initrd->zip)));
			return -NEWLAND_EINVAL;
		}
		memset(initrd_node->path, 0, PATH_MAX);
		strcpy(initrd_node->path, stat.m_filename);
		initrd_node->index = i;
		r = fs_node_create(&node, basename(initrd_node->path), 6 << (stat.m_is_directory ? FS_NODE_DIR : FS_NODE_FILE));
		if (r < 0) {
			kfree(initrd);
			kfree((*targetptr));
			kfree(initrd_node);
			return r;
		}
		initrd_node->initrd = initrd;
		node->impl = initrd_node;
		node->size = stat.m_uncomp_size;
		node->dev = source->rdev;
		if (stat.m_is_directory) node->opts.get_child = initrd_get_child;
		else node->opts.read = initrd_read;
		liblist_add(&initrd->nodes, node);
	}
	return 0;
}

static int initrd_umount(fs_node_t** targetptr) {
	struct initrd* initrd = (struct initrd*)((*targetptr)->impl);
	// TODO: free nodes
	mz_zip_reader_end(&initrd->zip);
	kfree(initrd);
	kfree(*targetptr);
	*targetptr = NULL;
	return 0;
}

static fs_opts_t initrd_opts = { .mount = initrd_mount, .umount = initrd_umount };

int initrd_init() {
	return register_fs("initrd", FS_VIRT, initrd_opts);
}
