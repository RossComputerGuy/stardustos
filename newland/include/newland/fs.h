/** \file include/newland/fs.h
 * \author Tristan Ross
 * \copyright GNU Public License 3.0
 * \brief Filesystem Node, Filesystem, and Mountpoint API
 * \since v0.1.0
 * \details Controls filesystem nodes, filesystems, and mountpoints
 */
#pragma once

#include <newland/time.h>
#include <sys/types.h>
#include <liblist.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>

/**
 * Filesystem node type: file
 */
#define FS_NODE_FILE 1

/**
 * Filesystem node type: directory
 */
#define FS_NODE_DIR 2

/**
 * Filesystem node type: unix socket
 */
#define FS_NODE_SOCK 3

/**
 * Filesystem node type: symlink
 */
#define FS_NODE_LINK 4

/**
 * Filesystem node type: block device
 */
#define FS_NODE_BLK 5

/**
 * Filesystem node type: character device
 */
#define FS_NODE_CHR 6

/**
 * Sets the type of the filesystem node
 *
 * @param[in] mode The variable to store the mode
 * @param[in] type The type of node
 */
#define FS_NODE_TYPE_SET(mode, type) (mode |= (6 << type))

/**
 * Gets the type of the filesystem node
 *
 * @param[in] mode The variable to store the mode
 * @return The type of node
 */
#define FS_NODE_TYPE_GET(mode) (mode >> 6)

struct fs_node;

/**
 * File Descriptor structure
 */
typedef struct {
	struct fs_node* node;
	gid_t gid;
	uid_t uid;
	pid_t pid;
	off_t offset;
	int mode;
} fd_t;

/**
 * Filesystem Node Operations
 *
 * @warning Never call these directly as some could not be set, use the filesystem node functions
 */
typedef struct {
	/**
	 * Opens a file descriptor
	 *
	 * @param[in] node The filesystem node
	 * @param[out] fd Pointer for the file descriptor
	 * @return Zero on success or a negative errno code on failure
	 */
	int (*open)(struct fs_node* node, fd_t* fd);

	/**
	 * Closes a file descriptor
	 *
	 * @param[in] node The filesystem node
	 * @param[out] fd Pointer for the file descriptor
	 * @return Zero on success or a negative errno code on failure
	 */
	int (*close)(struct fs_node* node, fd_t* fd);

	/**
	 * Gets a child node of this one, used only for directories
	 *
	 * @param[in] node The filesystem node
	 * @param[out] childptr The pointer to store the child node
	 * @param[in] index The index of the child to get
	 * @return Zero on success or a negative errno code
	 */
	int (*get_child)(struct fs_node* node, struct fs_node** childptr, size_t index);

	/**
	 * Creates a child node, used only for directories
	 *
	 * @param[in] node The filesystem node
	 * @param[out] childptr The pointer to store the child node
	 * @param[in] name The string to set as the name of the child node
	 * @param[in] mode The mode value of the child
	 * @return Zero on success or a negative errno code
	 */
	int (*mknode)(struct fs_node* node, struct fs_node** childptr, const char* name, mode_t mode);

	/**
	 * Removes a child node, used only for directories
	 *
	 * @param[in] node The filesystem node
	 * @param[out] childptr The pointer to the child node
	 * @return Zero on success or a negative errno code
	 */
	int (*rmnode)(struct fs_node* node, struct fs_node** childptr);

	/**
	 * Reads from the node
	 *
	 * @param[in] node The filesystem node
	 * @param[in] offset The offset to read from
	 * @param[in] buff The buffer to store the read data
	 * @param[in] size The number of bytes to read
	 * @return The number of bytes read on success or a negative errno code
	 */
	size_t (*read)(struct fs_node* node, off_t offset, void* buff, size_t size);

	/**
	 * Writes to the node
	 *
	 * @param[in] node The filesystem node
	 * @param[in] offset The offset to write at
	 * @param[in] buff The buffer to write into the node
	 * @param[in] size The size of the buffer to write into the node
	 * @return The number of bytes written on success or a negative errno code
	 */
	size_t (*write)(struct fs_node* node, off_t offset, const void* buff, size_t size);

	/**
	 * Run an I/O control operation on the node
	 *
	 * @param[in] node The filesystem node
	 * @param[in] req The request code
	 * @param[in] ap The va_list of arguments
	 * @return Can be anything above a negative number on success or a negative errno code on failure
	 */
	int (*ioctl)(struct fs_node* node, int req, va_list ap);
} fs_node_opts_t;

/**
 * Filesystem Node
 */
typedef struct fs_node {
	/**
	 * The name of the node
	 */
	const char name[NAME_MAX];

	/**
	 * The device value this node is from
	 */
	dev_t dev;

	/**
	 * The index node value
	 *
	 * @warning Never change this and never generate your own nodes, use the fs_node_create function when creating filesystem nodes
	 */
	ino_t ino;

	/**
	 * The node's mode which stores permissions and the type
	 */
	mode_t mode;

	/**
	 * The number of symlinks
	 */
	nlink_t nlink;

	/**
	 * The owner's UID
	 */
	uid_t uid;

	/**
	 * The owner's GID
	 */
	gid_t gid;

	/**
	 * The relative device value
	 */
	dev_t rdev;

	/**
	 * The size in bytes the contents of the node is
	 */
	size_t size;

	/**
	 * The size of the blocks used by the node
	 */
	blksize_t blksize;

	/**
	 * The number of blocks the node uses
	 */
	blkcnt_t blkcnt;

	/**
	 * The access time
	 */
	time_t atime;

	/**
	 * The time the node was last modified
	 */
	time_t mtime;

	/**
	 * The time of the node's creation
	 */
	time_t ctime;

	/**
	 * Filesystem Node operations
	 */
	fs_node_opts_t opts;

	/**
	 * Filesystem Node implementation pointer
	 */
	void* impl;
} fs_node_t;

/**
 * Filesystem type: pseudo
 */
#define FS_PSEUDO 0

/**
 * Filesystem type: virtual
 */
#define FS_VIRT 1

/**
 * Filesystem type: physical
 */
#define FS_PHYS 2

/**
 * Filesystem Operations
 *
 * @warning Never call these directly, use the mountpoint functions
 */
typedef struct {
	/**
	 * Mounts the filesystem
	 *
	 * @param[out] targetptr Pointer to store the root node of the filesystem
	 * @param[in] source The source node
	 * @param[in] flags Mount flags
	 * @param[in] data Mount data
	 */
	int (*mount)(fs_node_t** targetptr, fs_node_t* source, unsigned long flags, const void* data);

	/**
	 * Unmounts the filesystem
	 *
	 * @param[out] targetptr Pointer to the root node of the filesystem
	 */
	int (*umount)(fs_node_t** targetptr);
} fs_opts_t;

/**
 * A filesystem
 */
typedef struct fs {
	SLIST_ENTRY(struct fs) fs_list;

	/**
	 * Filesystem name
	 */
	const char name[NAME_MAX];

	/**
	 * Filesystem operations
	 */
	fs_opts_t opts;

	/**
	 * The filesystem type
	 */
	int type;
} fs_t;

/**
 * Flag which binds an already existsing directory to another directory
 */
#define MS_BIND (1 << 0)

/**
 * Mountpoint
 */
typedef struct mountpoint {
	SLIST_ENTRY(struct mountpoint) mp_list;

	/**
	 * Root node of the filesystem
	 */
	fs_node_t* rootnode;

	/**
	 * The name of the filesystem that was mounted
	 */
	const char fsname[NAME_MAX];

	/**
	 * The source path of the mountpoint
	 */
	const char source[PATH_MAX];

	/**
	 * The target path of the mountpoint
	 */
	const char target[PATH_MAX];

	/**
	 * Mount flags
	 */
	unsigned long flags;
} mountpoint_t;

/**
 * Creates a filesytem node
 *
 * @param[out] nodeptr The pointer to store the newly created node
 * @param[in] name The name of the node
 * @param[in] mode The mode of the node
 * @return Zero on success or a negative errno code
 */
int fs_node_create(fs_node_t** nodeptr, const char* name, mode_t mode);

/**
 * Opens a file descriptor
 *
 * @param[out] nodeptr The pointer to the filesystem node
 * @param[out] fd Pointer to the file descriptor
 * @return Zero on success or a negative errno code on failure
 */
int fs_node_open(fs_node_t** nodeptr, fd_t* fd);

/**
 * Closes a file descriptor
 *
 * @param[out] nodeptr The pointer to the filesystem node
 * @param[out] fd Pointer for the file descriptor
 * @return Zero on success or a negative errno code on failure
 */
int fs_node_close(fs_node_t** nodeptr, fd_t* fd);

/**
 * Reads from the node
 *
 * @param[in] nodeptr The pointer to the filesystem node
 * @param[in] offset The offset to write at
 * @param[in] buff The buffer to store the data read
 * @param[in] size The number of bytes to read
 * @return The number of bytes read on success or a negative errno code
 */
size_t fs_node_read(fs_node_t** nodeptr, off_t offset, void* buff, size_t size);

/**
 * Writes to the node
 *
 * @param[in] nodeptr The pointer to the filesystem node
 * @param[in] offset The offset to write at
 * @param[in] buff The buffer to write into the node
 * @param[in] size The size of the buffer to write into the node
 * @return The number of bytes written on success or a negative errno code
 */
size_t fs_node_write(fs_node_t** nodeptr, off_t offset, const void* buff, size_t size);

/**
 * Run an I/O control operation on the node
 *
 * @param[in] nodeptr The filesystem node
 * @param[in] req The request code
 * @param[in] ap The va_list of arguments
 * @return Can be anything above a negative number on success or a negative errno code on failure
 */
int fs_node_vioctl(fs_node_t** nodeptr, int req, va_list ap);

/**
 * Run an I/O control operation on the node
 *
 * @param[in] nodeptr The filesystem node
 * @param[in] req The request code
 * @return Can be anything above a negative number on success or a negative errno code on failure
 */
int fs_node_ioctl(fs_node_t** nodeptr, int req, ...);

/**
 * Resolves a child node from a path
 *
 * @param[in] nodeptr The pointer to the filesystem node
 * @param[out] foundptr The pointer to store the child node
 * @param[in] path The path of the node
 * @return Zero on success or a negative errno code
 */
int fs_node_resolve(fs_node_t** nodeptr, fs_node_t** foundptr, const char* path);

/**
 * Gets the number of registered filesystems
 *
 * @return A number
 */
size_t fs_count();

/**
 * Gets a filesystem by its index
 *
 * @param[in] i The index of the filesystem
 * @return A filesystem or NULL if not found
 */
fs_t* fs_get(size_t i);

/**
 * Gets a filesystem by its name
 *
 * @param[in] name The name of the filesystem
 * @return A filesystem or NULL if not found
 */
fs_t* fs_fromname(const char* name);

/**
 * Registers a filesystem
 *
 * @param[in] name The name of the filesystem
 * @param[in] type The type of the filesystem
 * @param[in] opts The filesystem operations
 * @return Zero on success or a negative errno code
 */
int register_fs(const char* name, int type, fs_opts_t opts);

/**
 * Unregisters a filesystem
 *
 * @param[in] name The name of the filesystem
 * @return Zero on success or a negative errno code
 */
int unregister_fs(const char* name);

/**
 * Resolves a path into a filesystem node
 *
 * @param[out] nodeptr The pointer to store the node
 * @param[in] path A path to a file
 * @return Zero on success or a negative errno code
 */
int fs_resolve(fs_node_t** nodeptr, const char* path);

/**
 * Gets the number of mountpoints
 *
 * @return A number
 */
size_t mountpoint_count();

/**
 * Gets a mountpoint by its source
 *
 * @param[in] src The source node path
 * @return A mountpoint or NULL if not found
 */
mountpoint_t* mountpoint_fromsrc(const char* src);

/**
 * Gets a mountpoint by its target
 *
 * @param[in] target The target node path
 * @return A mountpoint or NULL if not found
 */
mountpoint_t* mountpoint_fromtarget(const char* target);

/**
 * Creates a mountpoint using a source node
 *
 * @param[out] fsptr The pointer to the filesystem to use
 * @param[in] source The source node
 * @param[in] target The target path
 * @param[in] flags The mount flags
 * @param[in] data The mount data
 * @return Zero on success or a negative errno code
 */
int mountpoint_create_fromnode(fs_t** fsptr, fs_node_t* source, const char* target, unsigned long flags, const void* data);

/**
 * Creates a mountpoint
 *
 * @param[out] fsptr The pointer to the filesystem to use
 * @param[in] src The source path
 * @param[in] target The target path
 * @param[in] flags The mount flags
 * @param[in] data The mount data
 * @return Zero on success or a negative errno code
 */
int mountpoint_create(fs_t** fsptr, const char* src, const char* target, unsigned long flags, const void* data);

/**
 * Destroys a mountpoint
 *
 * @param[in] target The target path of the mountpoint
 * @return Zero on success or a negative errno code
 */
int mountpoint_destroy(const char* target);
