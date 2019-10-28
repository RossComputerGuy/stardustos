/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/boot/multiboot.h>
#include <newland/types.h>

void bootstrap_main(uint32_t magic, uint32_t mbaddr) {
  multiboot_info_t* mbi = (multiboot_info_t*)mbaddr;

  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    // TODO: panic
  }
}