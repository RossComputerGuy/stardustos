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
  return (((uint32_t)&__modules_start) - ((uint32_t)&__modules_end)) / sizeof(modinfo_t);
}

modinfo_t* module_fromid(const char* id) {
  for (size_t i = (uint32_t)&__modules_start; i < (uint32_t)&__modules_end; i += sizeof(modinfo_t)) {
    modinfo_t* modinfo = (modinfo_t*)i;
    if (!strcmp(modinfo->id, id)) return modinfo;
  }
  return NULL;
}

int modules_init() {
  for (size_t i = (uint32_t)&__modules_start; i < (uint32_t)&__modules_end; i += sizeof(modinfo_t)) {
    modinfo_t* modinfo = (modinfo_t*)i;
    printk(KLOG_INFO "Loading kernel module: %s\n", modinfo->id);
    int r = modinfo->init();
    if (r < 0) return r;
  }
  return 0;
}
