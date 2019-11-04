/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/error.h>
#include <newland/log.h>
#include <newland/module.h>
#include <newland/types.h>
#include <string.h>

extern uint32_t __modules_start;
extern uint32_t __modules_end;

int modules_init() {
  for (size_t i = (uint32_t)&__modules_start; i < (uint32_t)&__modules_end; i += sizeof(modinfo_t)) {
    modinfo_t* modinfo = (modinfo_t*)i;
    printk(KLOG_INFO "Loading kernel module: %s\n", modinfo->id);
    int r = modinfo->init();
    if (r < 0) return r;
  }
  return 0;
}
