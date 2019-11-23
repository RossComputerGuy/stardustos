/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/arch/proc.h>
#include <newland/errno.h>
#include <newland/fs.h>
#include <sys/newland.h>

int nl_getbuildprop(int prop, void* output, int* size) {
	switch (prop) {
#ifdef __nvk__
		case NL_PROP_ISNVK: return 1;
#else
		case NL_PROP_ISNVK: return 0;
#endif
		default: return -NEWLAND_EINVAL;
	}
	return 0;
}

pid_t nl_getpid() {
	proc_t* proc = process_curr();
	if (proc == NULL) return 0;
	return proc->id;
}

uid_t nl_getuid() {
	proc_t* proc = process_curr();
	if (proc == NULL) return 0;
	return proc->uid;
}

gid_t nl_getgid() {
	proc_t* proc = process_curr();
	if (proc == NULL) return 0;
	return proc->gid;
}

int nl_open(const char* path, int mode) {
	proc_t* proc = process_curr();
	if (proc == NULL) return -NEWLAND_EINVAL;
	int fd = -1;
	for (int i = 0; i < OPEN_MAX; i++) {
		if (proc->fd[i].node == NULL) {
			fd = i;
			break;
		}
	}
	if (fd == -1) return -NEWLAND_EMFILE;
	int r = fs_resolve(&proc->fd[fd].node, path);
	if (r < 0) return r;
	proc->fd[fd].mode = mode;
	proc->fd[fd].gid = proc->gid;
	proc->fd[fd].uid = proc->uid;
	proc->fd[fd].pid = proc->id;
	proc->fd[fd].offset = 0;
	if ((r = fs_node_open(&proc->fd[fd].node, &proc->fd[fd])) < 0) {
		proc->fd[fd].node = NULL;
		proc->fd[fd].mode = 0;
		proc->fd[fd].gid = 0;
		proc->fd[fd].uid = 0;
		return r;
	}
	return fd;
}

int nl_close(int fd) {
	proc_t* proc = process_curr();
	if (proc == NULL) return -NEWLAND_EINVAL;
	if (fd < 0 || fd >= OPEN_MAX) return -NEWLAND_EINVAL;
	int r = fs_node_close(&proc->fd[fd].node, &proc->fd[fd]);
	if (r < 0) return r;
	proc->fd[fd].node = NULL;
	proc->fd[fd].mode = 0;
	proc->fd[fd].gid = 0;
	proc->fd[fd].uid = 0;
	return 0;
}

size_t nl_write(int fd, const void* buff, size_t count) {
	proc_t* proc = process_curr();
	if (proc == NULL) return -NEWLAND_EINVAL;
	if (fd < 0 || fd >= OPEN_MAX) return -NEWLAND_EINVAL;
	/* TODO: permission checking */
	size_t r = fs_node_write(&proc->fd[fd].node, proc->fd[fd].offset, buff, count);
	if (r < 0) return r;
	proc->fd[fd].offset += r;
	return r;
}

size_t nl_read(int fd, void* buff, size_t count) {
	proc_t* proc = process_curr();
	if (proc == NULL) return -NEWLAND_EINVAL;
	if (fd < 0 || fd >= OPEN_MAX) return -NEWLAND_EINVAL;
	/* TODO: permission checking */
	size_t r = fs_node_read(&proc->fd[fd].node, proc->fd[fd].offset, buff, count);
	if (r < 0) return r;
	proc->fd[fd].offset += r;
	return r;
}
