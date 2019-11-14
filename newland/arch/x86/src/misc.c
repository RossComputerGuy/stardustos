/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/arch/misc.h>
#include <newland/log.h>
#include <string.h>

void halt() {
	while (1) asm volatile ("hlt");
}

void panic(const char* msg) {
	printk(KLOG_FATAL "panic: %s\n", msg);
	halt();
}
