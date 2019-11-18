#pragma once

#include <sys/types.h>
#include <liblist.h>
#include <limits.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>

typedef struct proc {
	SLIST_ENTRY(struct proc) proc_list;
	const char name[NAME_MAX];
	const char tty[NAME_MAX];
	const char cwd[PATH_MAX];
	pid_t id;
	pthread_t thread;
	gid_t gid;
	uid_t uid;
} proc_t;

size_t process_count();
proc_t* process_get(size_t i);
