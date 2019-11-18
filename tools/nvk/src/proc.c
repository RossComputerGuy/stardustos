/**
 * NewLand Virtual Kernel - (C) 2019 Tristan Ross
 */
#include <newland/dev/tty.h>
#include <nvk/proc.h>
#include <unicorn/unicorn.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

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

static void* proc_runner(void* arg) {
	proc_t* proc = (proc_t*)arg;
	proc->status = PROC_READY;
	void* ret = NULL;
	if (proc->isuser) {
		uc_engine* uc;
		uc_err err = uc_open(UC_ARCH_X86, UC_MODE_32, &uc);
		if (err != UC_ERR_OK) {
			fprintf(stderr, "Failed on uc_open() with error returned: %u\n", err);
			proc->status = PROC_ZOMBIE;
			return NULL;
		}
		uc_mem_map(uc, 0, 0xFFFFFF, UC_PROT_ALL);
		uc_mem_map_ptr(uc, 0x1000000, proc->prgsize, UC_PROT_ALL, proc->entry);
		uc_reg_write(uc, UC_X86_REG_EAX, &proc->impl);
		if ((err = uc_emu_start(uc, 0x1000000, 0x1000000 + proc->prgsize - 1, 0, 0)) != UC_ERR_OK) {
			fprintf(stderr, "Failed on uc_open() with error returned: %u\n", err);
			uc_close(uc);
			proc->status = PROC_ZOMBIE;
			return NULL;
		}
		uc_close(uc);
	} else ret = (proc->entry == NULL ? NULL : proc->entry(proc->impl));
	proc->status = PROC_ZOMBIE;
	return ret;
}

proc_t* proc_create(proc_t* parent, const char* name, int isuser) {
	proc_t* proc = malloc(sizeof(proc_t));
	if (proc == NULL) return NULL;
	memset(proc, 0, sizeof(proc_t));

	int parent_index = -1;
	if (parent != NULL) {
		for (size_t i = 0; i < parent->child_count; i++) {
			if (parent->child[i] == 0) {
				parent_index = i;
				break;
			}
		}
		if (parent_index == -1) {
			if (parent->child_count + 1 > CHILD_MAX) {
				errno = -ECHILD;
				return NULL;
			}
			parent_index = parent->child_count++;
		}
	}

	strncpy((char*)proc->name, name, NAME_MAX);
	proc->id = next_pid++;
	proc->isuser = isuser;
	proc->status = PROC_READY;

	if (parent == NULL) {
		strncpy((char*)proc->cwd, "/", PATH_MAX);
		if (tty_get(0) != NULL) strncpy((char*)proc->tty, tty_get(0)->name, NAME_MAX);
		proc->gid = proc->uid = proc->parent = 0;
	} else {
		strncpy((char*)proc->cwd, parent->cwd, PATH_MAX);
		proc->gid = parent->gid;
		proc->uid = parent->uid;
		proc->parent = parent->id;
		parent->child[parent_index] = proc->id;
		strncpy((char*)proc->tty, parent->tty, NAME_MAX);
	}
	// TODO: finish this
	return proc;
}

void proc_go(proc_t** procptr) {
	proc_t* proc = *procptr;
	pthread_create(&proc->thread, NULL, proc_runner, proc->impl);
}