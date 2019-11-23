/** \file arch/x86/include/newland/arch/proc.h
 * \author Tristan Ross
 * \copyright GNU Public License 3.0
 * \brief x86 multiprocessing
 * \since v0.1.0
 */
#pragma once

#ifndef __nvk__
#include <newland/arch/idt.h>
#include <newland/arch/mem.h>
#include <newland/fs.h>
#include <sys/types.h>
#include <liblist.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

#ifndef CHILD_MAX
#define CHILD_MAX 25
#endif

#ifndef OPEN_MAX
#define OPEN_MAX 512
#endif

#define SCHED_RECCOUNT 128

/**
 * Size of the process stack
 */
#define PROC_STACKSIZE 16384

/**
 * Zombie process
 */
#define PROC_ZOMBIE 0

/**
 * Process has finished
 */
#define PROC_FINISHED 1

/**
 * Process is running
 */
#define PROC_RUNNING 2

/**
 * Process is ready
 */
#define PROC_READY 3

typedef struct {
	uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
} proc_regs_t;

/**
 * Structure which represents a process
 * 
 * @todo This requires quite a bit of memory, we should try making it smaller
 */
typedef struct proc {
	SLIST_ENTRY(struct proc) proc_list;

	/**
	 * Process ID
	 */
	pid_t id;

	/**
	 * Process name
	 */
	const char name[256];

	/**
	 * Current working directory
	 */
	const char cwd[PATH_MAX];

	/**
	 * Process TTY
	 */
	const char tty[NAME_MAX];

	/**
	 * Process stack
	 */
	int stack[PROC_STACKSIZE];

	/**
	 * Process status
	 */
	int status;

	/**
	 * Process exit value
	 */
	int exitval;

	/**
	 * Is the process running in userspace?
	 */
	int isuser:1;

	/**
	 * Is the process signaling right now?
	 */
	int issignaling:1;

	/**
	 * Current process signal number
	 */
	uint8_t signum;

	/**
	 * Process registers
	 */
	proc_regs_t regs;

	/**
	 * Signal handler for the process
	 */
	void (*signal_handler)(uint8_t signum, void* data);

	/**
	 * FPU registers
	 */
	char fpu_regs[512];

	/**
	 * File descriptors
	 */
	fd_t fd[OPEN_MAX];

	/**
	 * Process GID
	 */
	gid_t gid;

	/**
	 * Process UID
	 */
	uid_t uid;

	/**
	 * Parent process ID
	 */
	pid_t parent;

	/**
	 * Child processes
	 */
	pid_t child[CHILD_MAX];

	/**
	 * Current child process count
	 */
	size_t child_count;
} proc_t;

/**
 * Gets CPU usage for a process
 *
 * @param[out] procptr The pointer to the process
 * @return The CPU usage of a process
 */
#define proc_getcpuusage(procptr) ((sched_getusage((*(procptr))->id) * 100) / SCHED_RECCOUNT)

/**
 * Switches the process registers
 *
 * @param[in] a The old registers
 * @param[in] b The new registers
 */
extern void proc_regswitch(proc_regs_t* a, proc_regs_t* b);

/**
 * Get the number of processes
 *
 * @return A number
 */
size_t process_count();

/**
 * Gets a process by its index
 *
 * @param[in] i The index of the process
 * @return A process or NULL if not found
 */
proc_t* process_get(size_t i);

/**
 * Gets a process by its PID
 *
 * @param[in] pid The process ID
 * @return A process or NULL if not found
 */
proc_t* process_frompid(pid_t pid);

/**
 * Gets the current running process
 * 
 * @return A process or NULL if not found
 */
proc_t* process_curr();

/**
 * Used by the scheduler to find the next process in which to execute
 *
 * @todo This function needs to check the process status
 * @return A pointer to a process or NULL if none was found
 */
proc_t* process_next();

/**
 * Creates a process
 *
 * @param[in] parent The pointer to the parent process
 * @param[in] name The process name
 * @param[in] isuser Set to true to run the process in userspace
 * @return A newly allocated process or NULL if it failed
 */
proc_t* proc_create(proc_t* parent, const char* name, int isuser);

/**
 * Destroys a process
 *
 * @param[out] procptr The pointer to the process
 * @return Zero on success or a negative errno code
 */
int proc_destroy(proc_t** procptr);

/**
 * Pushes data onto the process's stack
 *
 * @param[out] procptr The pointer to the process
 * @param[in] value The data to push
 * @param[in] size The size of the data
 * @return The new stack pointer
 */
int proc_pushstack(proc_t** procptr, const void* value, size_t size);

/**
 * Switches the page directory
 *
 * @param[out] procptr The pointer to the process
 * @param[in] pgdir The page directory to switch into
 * @return The old page directory
 */
page_dir_t* proc_switch_pgdir(proc_t** procptr, page_dir_t* pgdir);

/**
 * Enters a signal
 *
 * @param[out] procptr The pointer to the process
 * @param[in] signum The signal number
 * @param[in] data The data to send
 * @param[in] datasz The size of the data
 * @return Zero on success or a negative errno code
 */
int proc_sigenter(proc_t** procptr, uint8_t signum, void* data, size_t datasz);

/**
 * Leaves and restore the program from the signal
 *
 * @param[out] procptr The pointer to the process
 * @return Zero on sucess or a negative errno code
 */
int proc_sigleave(proc_t** procptr);

/**
 * Removes any zombie processes
 */
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

/**
 * Gets the process usage
 *
 * @param[in] pid The process ID
 * @return The process usage
 */
int sched_getusage(pid_t pid);

/**
 * Sets up the scheduler
 */
void sched_init();
#else
#include <nvk/proc.h>
#endif
