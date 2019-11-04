/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/error.h>
#include <newland/log.h>
#include <newland/module.h>
#include <newland/types.h>

extern uint32_t __modules_start;
extern uint32_t __modules_end;

size_t module_count() {
  return (((uint32_t)&__modules_end) - ((uint32_t)&__modules_start)) / sizeof(modinfo_t);
}

modinfo_t* module_fromid(const char* id) {
  size_t modcount = module_count();
  for (size_t i = 0; i < modcount; i++) {
    modinfo_t* mod = (modinfo_t*)((uint32_t)&__modules_start + (i * sizeof(modinfo_t)));
    if (!strcmp(mod->id, id)) return mod;
  }
  return NULL;
}

int modules_init() {
  size_t modcount = module_count();
  printk(KLOG_INFO "Loading %d kernel modules\n", modcount);
  for (size_t i = modcount; i != 0; i--) {
    modinfo_t* mod = (modinfo_t*)((uint32_t)&__modules_end - (i * sizeof(modinfo_t)));
    int r = mod->init();
    if (r < 0) return r;
  }
  return 0;
}
