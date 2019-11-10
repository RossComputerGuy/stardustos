/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/arch/fpu.h>
#include <newland/arch/gdt.h>
#include <newland/arch/idt.h>
#include <newland/arch/irq.h>
#include <newland/arch/mem.h>
#include <newland/arch/misc.h>
#include <newland/arch/proc.h>
#include <newland/arch/texcon.h>
#include <newland/arch/timer.h>
#include <newland/fs/initrd.h>
#include <newland/fs/procfs.h>
#include <newland/boot/multiboot.h>
#include <newland/log.h>
#include <newland/module.h>
#include <newland/time.h>
#include <newland/types.h>

extern time_t boot_time;

void bootstrap_main(uint32_t magic, uint32_t mbaddr) {
  boot_time = time(NULL);
  multiboot_info_t* mbi = (multiboot_info_t*)mbaddr;

  texcon_clear();

  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    panic("Multiboot magic check failed!");
  }

  gdt_init();
  idt_init();
  timer_init(50);
  fpu_init();
  mem_init(mbi);
  sched_init();

  int r = procfs_init();
  if (r < 0) panic("Failed to initialize procfs");

  r = initrd_init();
  if (r < 0) panic("Failed to initialize initrd");

  r = modules_init();
  if (r < 0) panic("Failed to load kernel modules");

  printk(KLOG_NOTICE "Bootstrapping has completed.\n");
}
