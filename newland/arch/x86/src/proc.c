/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/arch/fpu.h>
#include <newland/arch/irq.h>
#include <newland/arch/proc.h>
#include <newland/error.h>
#include <newland/kalloc.h>
#include <newland/signal.h>

SLIST_HEAD(proc_list, proc_t);
struct proc_list processes;
static pid_t next_pid = 1;
static pid_t curr_pid = 0;
static size_t proc_count = 0;
static uint32_t ticks = 0;
static int sched_rec[SCHED_RECCOUNT] = { 0 };

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
  return process_frompid(curr_pid);
}

proc_t* process_next() {
  proc_t* curr_proc = process_frompid(curr_pid);
  if (curr_proc == NULL) return NULL;
  if (SLIST_NEXT(curr_proc, proc_list) != NULL) return SLIST_NEXT(curr_proc, proc_list);
  return SLIST_FIRST(&processes);
}

int proc_create(proc_t** procptr, proc_t* parent, const char* name, int isuser) {
  proc_t* proc = (*procptr = kmalloc(sizeof(proc_t)));
  if (proc == NULL) return -ENOMEM;
  uint32_t irqflgs = irq_disable();
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
        irq_restore(irqflgs);
        return -ECHILD;
      }
      parent_index = parent->child_count++;
    }
  }

  proc->id = next_pid++;
  strncpy((char*)proc->name, name, 256);
  proc->status = PROC_READY;
  proc->isuser = isuser;

  if (parent == NULL) {
    strncpy((char*)proc->cwd, "/", PATH_MAX);
    proc->gid = proc->uid = proc->parent = 0;
  } else {
    strncpy((char*)proc->cwd, parent->cwd, PATH_MAX);
    proc->gid = parent->gid;
    proc->uid = parent->uid;
    proc->parent = parent->id;
    parent->child[parent_index] = proc->id;
  }

  if ((proc->isuser = isuser)) proc->pgdir = mem_alloc_pgdir();
  else proc->pgdir = get_krnlpgdir();

  memset(proc->stack, 0, PROC_STACKSIZE);
  proc->sp = (uint32_t)(&proc->stack[0] + PROC_STACKSIZE - 1);
  fpu_savectx(proc);

  SLIST_INSERT_HEAD(&processes, proc, proc_list);
  proc_count++;
  irq_restore(irqflgs);
  return proc->id;
}

int proc_destroy(proc_t** procptr) {
  uint32_t irqflgs = irq_disable();
  proc_t* proc = *procptr;
  if (proc->pgdir != get_krnlpgdir()) mem_free_pgdir(proc->pgdir);
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
  kfree(proc);
  SLIST_REMOVE(&processes, proc, proc_t, proc_list);
  proc_count--;
  irq_restore(irqflgs);
  return 0;
}

int proc_sigenter(proc_t** procptr, uint8_t signum, void* data, size_t datasz) {
  proc_t* proc = *procptr;
  if (proc->issignaling) return -EINPROGRESS;
  if (proc->signal_handler == NULL) {
    if (signum == SIGKILL || signum == SIGSEGV || signum == SIGSTKFLT || signum == SIGILL) {
      proc->status = PROC_ZOMBIE;
      return 0;
    }
    return -ENOSYS;
  }

  proc->issignaling = 1;
  proc->signum = signum;
  proc->sp -= sizeof(uint8_t);
  memcpy((void*)proc->sp, &signum, sizeof(uint8_t));
  if (datasz > 0 && data != NULL) {
    proc->sp -= datasz;
    memcpy((void*)proc->sp, data, datasz);
  }
  return 0;
}

int proc_sigleave(proc_t** procptr) {
  proc_t* proc = *procptr;
  if (!proc->issignaling) return -EINTR;
  proc->issignaling = 0;
  proc->signum = 0;
  return 0;
}

void proc_go(proc_t** procptr) {
  proc_t* proc = *procptr;
  regs_t regs;
  regs.eflags = 0x202;
  if (proc->issignaling) {
    if (proc->signal_handler != NULL) regs.eip = (uint32_t)proc->signal_handler;
  } else regs.eip = (uint32_t)proc->entry;
  regs.ebp = ((uint32_t)proc->stack + PROC_STACKSIZE);
  if (proc->isuser) {
    regs.cs = 0x18;
    regs.ds = 0x20;
    regs.es = 0x20;
    regs.fs = 0x20;
    regs.gs = 0x20;
  } else {
    regs.cs = 0x08;
    regs.ds = 0x10;
    regs.es = 0x10;
    regs.fs = 0x10;
    regs.gs = 0x10;
  }
  proc->sp -= sizeof(regs_t);
  memcpy((void*)proc->sp, &regs, sizeof(regs_t));
}

void processes_cleanup() {
  proc_t* proc;
  SLIST_FOREACH(proc, &processes, proc_list) {
    if (proc->status == PROC_ZOMBIE) proc_destroy(&proc);
  }
}

uint32_t schedule(uint32_t sp, regs_t regs) {
  proc_t* curr_proc = process_frompid(curr_pid);
  if (curr_proc == NULL) return sp;
  curr_proc->sp = sp;
  fpu_savectx(curr_proc);
  sched_rec[ticks % SCHED_RECCOUNT] = curr_pid;
  ticks++;
  proc_t* next_proc = process_next();
  if (next_proc != NULL) curr_pid = next_proc->id;
  paging_loaddir(curr_proc->pgdir);
  paging_invalidate_tlb();
  fpu_loadctx(next_proc);
  return next_proc->sp;
}

int sched_getusage(pid_t pid) {
  int count = 0;
  for (int i = 0; i < SCHED_RECCOUNT; i++) {
    if (sched_rec[i] == pid) count++;
  }
  return count;
}

static void proc_handle_interrrupt(regs_t regs) {
  proc_t* curr_proc = process_curr();
  if (curr_proc == NULL) return;
  switch (regs.int_no) {
    /* Invalid Opcode */
    case 0x06:
      proc_sigenter(&curr_proc, SIGILL, NULL, 0);
      break;
    /* FPU */
    case 0x10:
      proc_sigenter(&curr_proc, SIGFPE, NULL, 0);
      break;
    /* Stack Fault */
    case 0x0C:
      proc_sigenter(&curr_proc, SIGSTKFLT, NULL, 0);
      break;
    /* Page Fault */
    case 0x0E:
      proc_sigenter(&curr_proc, SIGSEGV, NULL, 0);
      break;
  }
}

void sched_init() {
  register_int_handler(0x06, proc_handle_interrrupt);
  register_int_handler(0x10, proc_handle_interrrupt);
  register_int_handler(0x0C, proc_handle_interrrupt);
  register_int_handler(0x0E, proc_handle_interrrupt);
  register_irq_handler(0x00, schedule);
}
