/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/arch/multiboot.h>
#include <newland/dev/block.h>
#include <newland/dev.h>
#include <errno.h>
#include <stddef.h>

/** Block device opts **/
static blkdev_opts_t mbinitrd_opts;

/** Normal device opts **/
static fs_node_opts_t mbimod_opts;

int mbi_modules_init(multiboot_info_t* mbi) {
  multiboot_module_t* mod = (multiboot_module_t*)mbi->mods_addr;
  size_t x = 0;
  for (size_t i = 0; i < mbi->mods_count; i++) {
    if (!strcmp((char*)mod->cmdline, "initrd")) {
/** Register as block device **/
       int r = register_blkdev("initrd", 1, mod->mod_end - mod->mod_start, mbinitrd_opts);
       if (r < 0) return r;
    } else {
/** Register as a multiboot module **/
      char name[7];
      strcpy(name, "mbmod");
      itoa(name + 5, 10, x);
      int r = register_device(MKDEV(DEVMAJ_MBMOD, x), name, mbimod_opts);
      if (r < 0) return r;
      x++;
    }
    mod = (multiboot_module_t*)((uint32_t)mod->mod_end + 1);
  }
  return 0;
}
