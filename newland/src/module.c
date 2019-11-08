/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/log.h>
#include <newland/module.h>
#include <newland/types.h>
#include <errno.h>

extern uintptr_t __modules_start;
extern uintptr_t __modules_end;

size_t module_count() {
  return (((uintptr_t)&__modules_end) - ((uintptr_t)&__modules_start)) / sizeof(modinfo_t);
}

modinfo_t* module_fromid(const char* id) {
  size_t modcount = module_count();
  modinfo_t* mods = (modinfo_t*)(&__modules_end);
  for (size_t i = 0; i < modcount; i++) {
    modinfo_t* mod = &mods[i];
    if (!strcmp(mod->id, id)) return mod;
  }
  return NULL;
}

int modules_init() {
  size_t modcount = module_count();
  printk(KLOG_INFO "Loading %d kernel modules\n", modcount);
  modinfo_t* mods = (modinfo_t*)(&__modules_end);
  for (size_t i = 0; i < modcount; i++) {
    modinfo_t* mod = &mods[i];
    int r = mod->init();
    if (r < 0) return r;
  }
  return 0;
}
