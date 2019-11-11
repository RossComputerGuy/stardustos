/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <libfile/elf.h>
#include <newland/arch/fpu.h>
#include <newland/arch/irq.h>
#include <newland/arch/proc.h>
#include <newland/dev/tty.h>
#include <newland/alloc.h>
#include <newland/log.h>
#include <newland/signal.h>
#include <errno.h>

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
  if (curr_pid == 0) return NULL;
  return process_frompid(curr_pid);
}

proc_t* process_next() {
  if (proc_count == 0) return NULL;
  proc_t* curr_proc = process_frompid(curr_pid);
  if (curr_proc == NULL) return SLIST_FIRST(&processes);
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

  for (size_t i = 0; i < OPEN_MAX; i++) {
    proc->fd[i].node = NULL;
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
  for (size_t i = 0; i < OPEN_MAX; i++) {
    if (proc->fd[i].node == NULL) continue;
    fs_node_close(&proc->fd[i].node, &proc->fd[i]);
  }
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

page_dir_t* proc_switch_pgdir(proc_t** procptr, page_dir_t* pgdir) {
  proc_t* proc = *procptr;
  page_dir_t* old = proc->pgdir;
  proc->pgdir = pgdir;
  paging_loaddir(pgdir);
  return old;
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

uint32_t schedule(uint32_t sp, regs_t* regs) {
  if (proc_count == 0) return sp;
  proc_t* curr_proc = process_frompid(curr_pid);
  proc_t* next_proc = process_next();
  if (curr_proc != NULL) {
    curr_proc->sp = sp;
    fpu_savectx(curr_proc);
    sched_rec[ticks % SCHED_RECCOUNT] = curr_pid;
    ticks++;
    curr_pid = next_proc->id;
  } else {
    curr_pid = 1;
  }
  if (curr_proc != NULL) paging_loaddir(curr_proc->pgdir);
  paging_invalidate_tlb();
  fpu_loadctx(next_proc);
  printk(KLOG_INFO "proc: switching context %d\n", next_proc->id);
  return next_proc->sp;
}

int proc_exec(const char* path, const char** argv) {
  fs_node_t* node;
  int r = fs_resolve(&node, path);
  if (r < 0) return r;

  elf_header_t hdr;
  r = fs_node_read(&node, 0, &hdr, sizeof(elf_header_t));
  if (r < 0) return r;

  if (!(elf_isvalid(&hdr) && hdr.machine == 3)) return -EINVAL;

  uint32_t eflags = irq_disable();
  proc_t* proc;
  proc_t* curr = process_curr();
  r = proc_create(&proc, curr, path, 1);
  if (r < 0) {
    irq_restore(eflags);
    return r;
  }

  proc->entry = (void*)hdr.entry;

  proc->sp -= strlen(path) + 1;
  memcpy((void*)proc->sp, path, strlen(path) + 1);

  int argc = 0;
  if (argv != NULL) {
    while (argv[argc] != NULL) {
      proc->sp -= strlen(argv[argc]) + 1;
      memcpy((void*)proc->sp, argv[argc], strlen(argv[argc]) + 1);
      argc++;
    }
  }
  argc++;

  proc->sp -= sizeof(int);
  memcpy((void*)proc->sp, &argc, sizeof(int));

  elf_program_t prog;
  for (int i = 0; i < hdr.phnum; i++) {
    r = fs_node_read(&node, hdr.phoff + (hdr.phentsize * i), &prog, sizeof(elf_program_t));
    if (r < 0) {
      proc_destroy(&proc);
      irq_restore(eflags);
      return r;
    }
    if (prog.vaddr >= 0x100000) {
      page_dir_t* oldpgdir = proc_switch_pgdir(&curr, proc->pgdir);
      paging_loaddir(proc->pgdir);
      mem_map(proc->pgdir, prog.vaddr, PAGE_ALIGN_UP(prog.memsz) / PAGE_SIZE, 1, 1);
      memset((void*)prog.vaddr, 0, prog.memsz);

      r = fs_node_read(&node, prog.offset, (void*)prog.vaddr, prog.filesz);
      if (r < 0) {
        proc_destroy(&proc);
        irq_restore(eflags);
        return r;
      }
      proc_switch_pgdir(&curr, oldpgdir);
    }
  }

  proc_go(&proc);

  irq_restore(eflags);
  return proc->id;
}

int sched_getusage(pid_t pid) {
  int count = 0;
  for (int i = 0; i < SCHED_RECCOUNT; i++) {
    if (sched_rec[i] == pid) count++;
  }
  return count;
}

static void proc_handle_interrrupt(regs_t* regs) {
  proc_t* curr_proc = process_curr();
  if (curr_proc == NULL) return;
  switch (regs->int_no) {
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
