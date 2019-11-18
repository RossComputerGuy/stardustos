/**
 * NewLand Virtual Kernel - (C) 2019 Tristan Ross
 */
#include <nvk/proc.h>
#include <liblist.h>
#include <pthread.h>

SLIST_HEAD(proc_list, proc_t);
static struct proc_list processes;
static pid_t next_pid = 1;
static pid_t curr_pid = 0;
static size_t proc_count = 0;

size_t process_count() {
	return proc_count;
}

proc_t* process_get(size_t i) {
	proc_t* proc = NULL;
	size_t index = 0;
	SLIST_FOREACH(proc, &processes, proc_list) {
		if (index == i) return proc;
		index++;
	}
	return NULL;
}
