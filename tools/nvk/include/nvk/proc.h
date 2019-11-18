#pragma once

#include <sys/types.h>
#include <liblist.h>
#include <stdint.h>
#include <string.h>

typedef struct proc {
	SLIST_ENTRY(struct proc) proc_list;
	pid_t id;
} proc_t;

size_t process_count();
proc_t* process_get(size_t i);
