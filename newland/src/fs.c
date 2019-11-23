/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/alloc.h>
#include <newland/fs.h>
#include <newland/errno.h>
#include <libgen.h>

/** Filesystem Node **/
static ino_t next_inode = 0;

int fs_node_create(fs_node_t** nodeptr, const char* name, mode_t mode) {
	if (strlen(name) > NAME_MAX) return -NEWLAND_ENAMETOOLONG;
	fs_node_t* node = (*nodeptr = kmalloc(sizeof(fs_node_t)));
	if (node == NULL) return -NEWLAND_ENOMEM;
	strcpy((char*)node->name, name);
	node->mode = mode;
	node->ino = next_inode++;
	return node->ino;
}

int fs_node_open(fs_node_t** nodeptr, fd_t* fd) {
	fs_node_t* node = *nodeptr;
	if (node->opts.open == NULL) return 0;
	return node->opts.open(node, fd);
}

int fs_node_close(fs_node_t** nodeptr, fd_t* fd) {
	fs_node_t* node = *nodeptr;
	if (node->opts.close == NULL) return 0;
	return node->opts.close(node, fd);
}

size_t fs_node_read(fs_node_t** nodeptr, off_t offset, void* buff, size_t size) {
	fs_node_t* node = *nodeptr;
	int type = FS_NODE_TYPE_GET(node->mode);
	if (node->opts.read == NULL) return -NEWLAND_ENOSYS;
	if (type == FS_NODE_LINK) {
		char path[PATH_MAX];
		int r = node->opts.read(node, 0, path, node->size);
		if (r < 0) return r;
		fs_node_t* parentnode;
		r = fs_resolve(&parentnode, path);
		if (r < 0) return r;
		return fs_node_read(&parentnode, offset, buff, size);
	} else if (type == FS_NODE_DIR) return -NEWLAND_EISDIR;
	return node->opts.read(node, offset, buff, size);
}

size_t fs_node_write(fs_node_t** nodeptr, off_t offset, const void* buff, size_t size) {
	fs_node_t* node = *nodeptr;
	int type = FS_NODE_TYPE_GET(node->mode);
	if (node->opts.read == NULL) return -NEWLAND_ENOSYS;
	if (node->opts.write == NULL) return -NEWLAND_ENOSYS;
	if (type == FS_NODE_LINK) {
		char path[PATH_MAX];
		int r = node->opts.read(node, 0, path, node->size);
		if (r < 0) return r;
		fs_node_t* parentnode;
		r = fs_resolve(&parentnode, path);
		if (r < 0) return r;
		return fs_node_write(&parentnode, offset, buff, size);
	} else if (type == FS_NODE_DIR) return -NEWLAND_EISDIR;
	return node->opts.write(node, offset, buff, size);
}

int fs_node_vioctl(fs_node_t** nodeptr, int req, va_list ap) {
	fs_node_t* node = *nodeptr;
	int type = FS_NODE_TYPE_GET(node->mode);
	if (node->opts.read == NULL) return -NEWLAND_ENOSYS;
	if (node->opts.ioctl == NULL) return -NEWLAND_ENOSYS;
	if (type == FS_NODE_LINK) {
		char path[PATH_MAX];
		int r = node->opts.read(node, 0, path, node->size);
		if (r < 0) return r;
		fs_node_t* parentnode;
		r = fs_resolve(&parentnode, path);
		if (r < 0) return r;
		return fs_node_vioctl(&parentnode, req, ap);
	} else if (type == FS_NODE_DIR) return -NEWLAND_EISDIR;
	return node->opts.ioctl(node, req, ap);
}

int fs_node_ioctl(fs_node_t** nodeptr, int req, ...) {
	va_list ap;
	va_start(ap, req);
	int r = fs_node_vioctl(nodeptr, req, ap);
	va_end(ap);
	return r;
}

int fs_node_resolve(fs_node_t** nodeptr, fs_node_t** foundptr, const char* path) {
	if (path[0] == '/') path++;
	size_t plen = strlen(dirname((char*)path));
	fs_node_t* node = *nodeptr;
	fs_node_t* tmpnode = NULL;
	size_t i = 0;
	int r;
	while ((r = node->opts.get_child(node, &tmpnode, i++)) == 0) {
		if (plen > 1) {
			if (!strncmp(tmpnode->name, path, plen)) return fs_node_resolve(&tmpnode, foundptr, path + plen);
		} else {
			if (!strcmp(tmpnode->name, path)) {
				*foundptr = tmpnode;
				return 0;
			}
		}
	}
	return -NEWLAND_ENOENT;
}

/** Filesystem stuff **/

SLIST_HEAD(fs_list, fs_t);
static struct fs_list filesystems = { NULL };
static size_t filesystem_count = 0;

size_t fs_count() {
	return filesystem_count;
}

fs_t* fs_get(size_t i) {
	fs_t* fs = NULL;
	size_t index = 0;
	SLIST_FOREACH(fs, &filesystems, fs_list) {
		if (index == i) return fs;
		index++;
	}
	return NULL;
}

fs_t* fs_fromname(const char* name) {
	fs_t* fs = NULL;
	SLIST_FOREACH(fs, &filesystems, fs_list) {
		if (!strcmp(fs->name, name)) return fs;
	}
	return NULL;
}

int register_fs(const char* name, int type, fs_opts_t opts) {
	if (fs_fromname(name) != NULL) return -NEWLAND_EEXIST;
	if (strlen(name) > NAME_MAX) return -NEWLAND_ENAMETOOLONG;
	fs_t* fs = kmalloc(sizeof(fs_t));
	if (fs == NULL) return -NEWLAND_ENOMEM;
	strcpy((char*)fs->name, name);
	fs->type = type;
	fs->opts = opts;
	SLIST_INSERT_HEAD(&filesystems, fs, fs_list);
	filesystem_count++;
	return 0;
}

int unregister_fs(const char* name) {
	fs_t* fs = fs_fromname(name);
	if (fs == NULL) return -NEWLAND_ENOENT;
	SLIST_REMOVE(&filesystems, fs, fs_t, fs_list);
	filesystem_count--;
	kfree(fs);
	return 0;
}

/** Mount Points **/

SLIST_HEAD(mp_list, mountpoint_t);
static struct mp_list mountpoints = { NULL };
static size_t mp_count = 0;

int fs_resolve(fs_node_t** nodeptr, const char* path) {
	if (path == NULL) return -NEWLAND_EINVAL;
	if (path[0] == '/') path++;
	mountpoint_t* mp = NULL;
	fs_node_t* root = NULL;
	SLIST_FOREACH(mp, &mountpoints, mp_list) {
		if (!strncmp(path, mp->target, strlen(mp->target))) {
			root = mp->rootnode;
			break;
		}
	}
	if (root == NULL) {
		mp = mountpoint_fromtarget("/");
		if (mp == NULL) return -NEWLAND_EINVAL;
		root = mp->rootnode;
	}
	if (root == NULL) return -NEWLAND_EINVAL;
	path += strlen(mp->target);
	return fs_node_resolve(&root, nodeptr, path);
}

size_t mountpoint_count() {
	return mp_count;
}

mountpoint_t* mountpoint_fromsrc(const char* src) {
	mountpoint_t* mp = NULL;
	SLIST_FOREACH(mp, &mountpoints, mp_list) {
		if (!strcmp(mp->source, src)) return mp;
	}
	return NULL;
}

mountpoint_t* mountpoint_fromtarget(const char* target) {
	mountpoint_t* mp = NULL;
	SLIST_FOREACH(mp, &mountpoints, mp_list) {
		if (!strcmp(mp->target, target)) return mp;
	}
	return NULL;
}

int mountpoint_create_fromnode(fs_t** fsptr, fs_node_t* source, const char* target, unsigned long flags, const void* data) {
	if (!(flags & MS_BIND) && (fsptr == NULL || *fsptr == NULL)) return -NEWLAND_EINVAL;
	if (target[0] == '/') target++;
	mountpoint_t* mp = kmalloc(sizeof(mountpoint_t));
	if (mp == NULL) return -NEWLAND_ENOMEM;
	strcpy((char*)mp->target, target);
	mp->flags = flags;
	if (!(flags & MS_BIND)) {
		strcpy((char*)mp->fsname, (*fsptr)->name);
		int r = (*fsptr)->opts.mount(&mp->rootnode, source, flags, data);
		if (r < 0) {
			kfree(mp);
			return r;
		}
	} else mp->rootnode = source;
	SLIST_INSERT_HEAD(&mountpoints, mp, mp_list);
	mp_count++;
	return 0;
}

int mountpoint_create(fs_t** fsptr, const char* src, const char* target, unsigned long flags, const void* data) {
	if (!(flags & MS_BIND) && (fsptr == NULL || *fsptr == NULL)) return -NEWLAND_EINVAL;
	if ((src != NULL && mountpoint_fromsrc(src) != NULL) || mountpoint_fromtarget(target) != NULL) return -NEWLAND_EBUSY;
	if (target[0] == '/') target++;
	fs_node_t* sourcenode = NULL;
	int r = fs_resolve(&sourcenode, src);
	if (r < 0) return r;
	r = mountpoint_create_fromnode(fsptr, sourcenode, target, flags, data);
	if (r < 0) return r;
	mountpoint_t* mp = mountpoint_fromtarget(target);
	strcpy((char*)mp->source, src);
	return 0;
}

int mountpoint_destroy(const char* target) {
	mountpoint_t* mp = mountpoint_fromtarget(target);
	if (mp == NULL) return -NEWLAND_EINVAL;
	SLIST_REMOVE(&mountpoints, mp, mountpoint_t, mp_list);
	mp_count--;
	kfree(mp);
	return 0;
}
