/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/error.h>
#include <newland/fs.h>
#include <newland/kalloc.h>
#include <newland/limits.h>

/** Filesystem Node **/
static ino_t next_inode = 0;

int fs_node_create(fs_node_t** nodeptr, const char* name, mode_t mode) {
  if (strlen(name) > NAME_MAX) return -ENAMETOOLONG;
  fs_node_t* node = (*nodeptr = kmalloc(sizeof(fs_node_t)));
  if (node == NULL) return -ENOMEM;
  strcpy((char*)node->name, name);
  node->mode = mode;
  node->ino = next_inode++;
  return node->ino;
}

size_t fs_node_read(fs_node_t** nodeptr, off_t offset, void* buff, size_t size) {
  fs_node_t* node = *nodeptr;
  int type = FS_NODE_TYPE_GET(node->mode);
  if (type == FS_NODE_LINK) {
    // TODO: find the parent and use it
  } else if (type == FS_NODE_DIR) return -EISDIR;
  if (node->opts.read == NULL) return -ENOSYS;
  return node->opts.read(node, offset, buff, size);
}

size_t fs_node_write(fs_node_t** nodeptr, off_t offset, const void* buff, size_t size) {
  fs_node_t* node = *nodeptr;
  int type = FS_NODE_TYPE_GET(node->mode);
  if (type == FS_NODE_LINK) {
    // TODO: find the parent and use it
  } else if (type == FS_NODE_DIR) return -EISDIR;
  if (node->opts.read == NULL) return -ENOSYS;
  return node->opts.write(node, offset, buff, size);
}

int fs_node_vioctl(fs_node_t** nodeptr, int req, va_list ap) {
  fs_node_t* node = *nodeptr;
  int type = FS_NODE_TYPE_GET(node->mode);
  if (type == FS_NODE_LINK) {
    // TODO: find the parent and use it
  } else if (type == FS_NODE_DIR) return -EISDIR;
  if (node->opts.ioctl == NULL) return -ENOSYS;
  return node->opts.ioctl(node, req, ap);
}

int fs_node_ioctl(fs_node_t** nodeptr, int req, ...) {
  va_list ap;
  va_start(ap, req);
  int r = fs_node_vioctl(nodeptr, req, ap);
  va_end(ap);
  return r;
}

/** Filesystem stuff **/

/** Mount Points **/

SLIST_HEAD(mp_list, mountpoint_t);
static struct mp_list mountpoints = { NULL };
static size_t mp_count = 0;

int fs_resolve_node(fs_node_t** nodeptr, const char* path) {
  if (path[0] == '/') path++;
  if (path[strlen(path) - 1] == '/') memset((void*)(path + (strlen(path) - 1)), 0, sizeof(char));
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
    if (mp == NULL) return -EINVAL;
    root = mp->rootnode;
  }
  if (root == NULL) return -EINVAL;
  // TODO: find a path using the node
  return 0;
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

int mountpoint_create(fs_t** fsptr, const char* src, const char* target, unsigned long flags, const void* data) {
  if (!(flags & MS_BIND) && (fsptr == NULL || *fsptr == NULL)) return -EINVAL;
  if (mountpoint_fromsrc(src) != NULL || mountpoint_fromtarget(target) != NULL) return -EBUSY;
  if (target[0] == '/') target++;
  if (target[strlen(target) - 1] == '/') memset((void*)(target + (strlen(target) - 1)), 0, sizeof(char));
  mountpoint_t* mp = kmalloc(sizeof(mountpoint_t));
  if (mp == NULL) return -ENOMEM;
  strcpy((char*)mp->source, src);
  strcpy((char*)mp->target, target);
  mp->flags = flags;
  if (!(flags & MS_BIND)) {
    strcpy((char*)mp->fsname, (*fsptr)->name);
    // TODO: resolve the path of the target inode
    int r = (*fsptr)->mount(&mp->rootnode, NULL, flags, data);
    if (r < 0) {
      kfree(mp);
      return r;
    }
  } else {
    // TODO: set the root node to the resolved source inode
  }
  SLIST_INSERT_HEAD(&mountpoints, mp, mp_list);
  mp_count++;
  return 0;
}