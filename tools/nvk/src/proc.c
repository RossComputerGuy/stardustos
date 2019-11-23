/**
 * NewLand Virtual Kernel - (C) 2019 Tristan Ross
 */
#include <libfile/elf.h>
#include <newland/dev/tty.h>
#include <newland/errno.h>
#include <newland/fs.h>
#include <newland/syscall.h>
#include <nvk/emulator.h>
#include <nvk/proc.h>
#include <sys/mman.h>
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

proc_t* process_frompid(pid_t pid) {
	proc_t* proc = NULL;
	SLIST_FOREACH(proc, &processes, proc_list) {
		if (proc->id == pid) return proc;
	}
	return NULL;
}

proc_t* process_curr() {
	if (proc_count == 0) return NULL;
	if (curr_pid == 0) return NULL;
	return process_frompid(curr_pid);
}

proc_t* process_next() {
	if (proc_count == 0) return NULL;
	if (curr_pid == 0) return NULL;
	proc_t* curr_proc = process_frompid(curr_pid);
	if (curr_proc == NULL) return SLIST_FIRST(&processes);
	if (SLIST_NEXT(curr_proc, proc_list) != NULL) return SLIST_NEXT(curr_proc, proc_list);
	return SLIST_FIRST(&processes);
}

static void* proc_runner(void* arg) {
	proc_t* proc = (proc_t*)arg;
	proc->status = PROC_READY;
	void* ret = NULL;
	curr_pid = proc->id;
	if (proc->isuser) {
		uc_err err = nvk_emu(proc->prog, proc->prgsize, proc->impl);
		if (err != UC_ERR_OK) {
			proc->status = PROC_ZOMBIE;
			return NULL;
		}
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
				errno = ECHILD;
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

	SLIST_INSERT_HEAD(&processes, proc, proc_list);
	proc_count++;
	if (proc_count == 1 && curr_pid == 0) curr_pid = proc->id;
	return proc;
}

int proc_destroy(proc_t** procptr) {
	proc_t* proc = *procptr;
	if (proc->prog != NULL) free(proc->prog);
	if (proc->parent != 0) {
		proc_t* parent = process_frompid(proc->parent);
		for (int i = 0; i < parent->child_count; i++) {
			if (parent->child[i] == proc->id) {
				parent->child[i] = 0;
				parent->child_count--;
				break;
			}
		}
	}
	SLIST_REMOVE(&processes, proc, proc_t, proc_list);
	proc_count--;
	free(proc);
	return 0;
}

void proc_go(proc_t** procptr) {
	proc_t* proc = *procptr;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setstack(&attr, proc->stack, PROC_STACKSIZE);
	pthread_create(&proc->thread, &attr, proc_runner, proc);
	pthread_join(proc->thread, NULL);
}

int proc_exec(const char* path, const char** argv) {
	fs_node_t* node;
	int r = fs_resolve(&node, path);
	if (r < 0) return r;

	void* prog = malloc(node->size);
	if (prog == NULL) return -NEWLAND_ENOMEM;
	size_t total = 0;
	while (total < node->size) {
		r = fs_node_read(&node, total, prog, node->size);
		if (r < 0) {
			free(prog);
			return r;
		}
		total += r;
	}

	if (!elf_isvalid((elf_header_t*)prog)) {
		free(prog);
		return -NEWLAND_EINVAL;
	}

	proc_t* curr = process_curr();
	proc_t* proc = proc_create(curr, path, 1);
	if (proc == NULL) {
		free(prog);
		return -NEWLAND_EINVAL;
	}

	proc->prog = prog;

	proc_go(&proc);
	return proc->id;
}