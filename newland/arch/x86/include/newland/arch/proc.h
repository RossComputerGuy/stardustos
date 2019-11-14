/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#pragma once

#include <newland/arch/idt.h>
#include <newland/arch/mem.h>
#include <newland/fs.h>
#include <newland/limits.h>
#include <newland/types.h>
#include <liblist.h>
#include <string.h>

#define SCHED_RECCOUNT 128

#define PROC_STACKSIZE 16384

#define PROC_ZOMBIE 0
#define PROC_FINISHED 1
#define PROC_RUNNING 2
#define PROC_READY 3

/**
 * Structure which represents a process
 * 
 * @todo This requires quite a bit of memory, we should try making it smaller
 */
typedef struct proc {
	SLIST_ENTRY(struct proc) proc_list;
	pid_t id;

	const char name[256];
	const char cwd[PATH_MAX];
	const char tty[NAME_MAX];

	int status;
	int exitval;
	int isuser:1;
	int issignaling:1;

	uint32_t sp;
	uint8_t signum;
	int stack[PROC_STACKSIZE];
	void (*entry)();
	void (*signal_handler)(uint8_t signum, void* data);
	page_dir_t* pgdir;
	char fpu_regs[512];

	fd_t fd[OPEN_MAX];

	gid_t gid;
	uid_t uid;

	pid_t parent;
	pid_t child[CHILD_MAX];
	size_t child_count;
} proc_t;

/**
 * Gets CPU usage for a process
 *
 * @param[out] procptr The pointer to the process
 * @return The CPU usage of a process
 */
#define proc_getcpuusage(procptr) ((sched_getusage((*(procptr))->id) * 100) / SCHED_RECCOUNT)

size_t process_count();
proc_t* process_get(size_t i);
proc_t* process_frompid(pid_t pid);
proc_t* proccess_curr();

/**
 * Used by the scheduler to find the next process in which to execute
 *
 * @todo This function needs to check the process status
 * @return A pointer to a process or NULL if none was found
 */
proc_t* process_next();

int proc_create(proc_t** procptr, proc_t* parent, const char* name, int isuser);
int proc_destroy(proc_t** procptr);
page_dir_t* proc_switch_pgdir(proc_t** procptr, page_dir_t* pgdir);
int proc_sigenter(proc_t** procptr, uint8_t signum, void* data, size_t datasz);
int proc_sigleave(proc_t** procptr);
void proc_go(proc_t** procptr);

void processes_cleanup();

/**
 * Executes a process from an executable file
 *
 * @bug The process information is forgotten when the schedule function is called (#6)
 * @param[in] path The path of the executable
 * @param[in] argv The arguments to pass, terminated with NULL
 * @return Zero on success or a negative errno code
 */
int proc_exec(const char* path, const char** argv);

int sched_getusage(pid_t pid);
void sched_init();
