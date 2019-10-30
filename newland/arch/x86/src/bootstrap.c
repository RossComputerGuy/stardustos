/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/arch/gdt.h>
#include <newland/arch/idt.h>
#include <newland/arch/mem.h>
#include <newland/arch/misc.h>
#include <newland/arch/texcon.h>
#include <newland/arch/timer.h>
#include <newland/boot/multiboot.h>
#include <newland/types.h>

void bootstrap_main(uint32_t magic, uint32_t mbaddr) {
  multiboot_info_t* mbi = (multiboot_info_t*)mbaddr;

  texcon_clear();

  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    panic("Multiboot magic check failed!");
  }

  gdt_init();
  idt_init();
  timer_init(50);
  paging_init();
}
