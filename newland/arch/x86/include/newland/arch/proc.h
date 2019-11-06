/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#pragma once

#include <newland/arch/idt.h>
#include <newland/arch/mem.h>
#include <newland/fs.h>
#include <newland/limits.h>
#include <newland/list.h>
#include <newland/types.h>
#include <string.h>

#define SCHED_RECCOUNT 128

#define PROC_STACKSIZE 16384

#define PROC_ZOMBIE 0
#define PROC_FINISHED 1
#define PROC_RUNNING 2
#define PROC_READY 3

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

#define proc_getcpuusage(procptr) ((sched_getusage((*(procptr))->id) * 100) / SCHED_RECCOUNT)

size_t process_count();
proc_t* process_get(size_t i);
proc_t* process_frompid(pid_t pid);
proc_t* proccess_curr();
proc_t* process_next();

int proc_create(proc_t** procptr, proc_t* parent, const char* name, int isuser);
int proc_destroy(proc_t** procptr);
int proc_sigenter(proc_t** procptr, uint8_t signum, void* data, size_t datasz);
int proc_sigleave(proc_t** procptr);
void proc_go(proc_t** procptr);

void processes_cleanup();

int sched_getusage(pid_t pid);
void sched_init();
