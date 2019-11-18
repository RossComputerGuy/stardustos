#pragma once

#include <sys/types.h>
#include <liblist.h>
#include <limits.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>

#define CHILD_MAX 128

#define PROC_ZOMBIE 0
#define PROC_FINISHED 1
#define PROC_RUNNING 2
#define PROC_READY 3

typedef struct proc {
	SLIST_ENTRY(struct proc) proc_list;
	const char name[NAME_MAX];
	const char tty[NAME_MAX];
	const char cwd[PATH_MAX];
	int status;
	int exitval;
	int isuser:1;
	int issignaling:1;
	pid_t id;
	pthread_t thread;
	gid_t gid;
	uid_t uid;
	pid_t parent;
	pid_t child[CHILD_MAX];
	size_t child_count;
	void* (*entry)(void* impl);
	void* impl;
	size_t prgsize;
} proc_t;

size_t process_count();
proc_t* process_get(size_t i);

proc_t* proc_create(proc_t* parent, const char* name, int isuser);
void proc_go(proc_t** procptr);