/**
 * NewLand kernel - (C) 2019 Tristan Ross
 */
#include <newland/errno.h>
#include <newland/syscall.h>
#include <sys/newland.h>
#include <stddef.h>

const static syscall_t syscall_handlers[] = {
#include <newland/__syscall_handlers.h>
};

#define N_SYSCALLS (sizeof(syscall_handlers) / sizeof(syscall_t))

int syscall_run(int i, void* arg0, void* arg1, void* arg2, void* arg3, void* arg4) {
	if (i < 0 || i >= N_SYSCALLS) return -NEWLAND_EINVAL;
	if (syscall_handlers[i] == NULL) return -NEWLAND_ENOSYS;
	return syscall_handlers[i](arg0, arg1, arg2, arg3, arg4);
}