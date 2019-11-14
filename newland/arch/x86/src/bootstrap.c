/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/arch/fpu.h>
#include <newland/arch/gdt.h>
#include <newland/arch/idt.h>
#include <newland/arch/irq.h>
#include <newland/arch/mem.h>
#include <newland/arch/misc.h>
#include <newland/arch/multiboot.h>
#include <newland/arch/proc.h>
#include <newland/arch/timer.h>
#include <newland/fs/devfs.h>
#include <newland/fs/initrd.h>
#include <newland/fs/procfs.h>
#include <newland/boot/multiboot.h>
#include <newland/dev.h>
#include <newland/log.h>
#include <newland/module.h>
#include <newland/time.h>
#include <sys/types.h>

extern time_t boot_time;

void bootstrap_main(uint32_t magic, uint32_t mbaddr) {
	boot_time = time(NULL);
	multiboot_info_t* mbi = (multiboot_info_t*)mbaddr;

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		panic("Multiboot magic check failed!");
	}

	gdt_init();
	idt_init();
	timer_init(50);
	fpu_init();
	mem_init(mbi);
	sched_init();

	int r = mbi_modules_init(mbi);
	if (r < 0) panic("Failed to create devices for multiboot modules");

	r = procfs_init();
	if (r < 0) panic("Failed to initialize procfs");

	r = initrd_init();
	if (r < 0) panic("Failed to initialize initrd");

	r = modules_init();
	if (r < 0) panic("Failed to load kernel modules");

	r = devfs_init();
	if (r < 0) panic("Failed to load kernel modules");

	printk(KLOG_NOTICE "mounting initrd\n");

	device_t* device = device_fromname("mbmod00");
	if (device == NULL) panic("No initrd device was attached");

	size_t devi = device_indexof(device);
	if (devi == -1) panic("Failed to find the index of the initrd device");

	fs_node_t* initrd = devfs_get(devi);
	if (initrd == NULL) panic("Failed to get devfs initrd device node");

	fs_t* fs = fs_fromname("initrd");
	if (fs == NULL) panic("Failed to find fs: initrd");

	r = mountpoint_create_fromnode(&fs, initrd, "/", 0, NULL);
	if (r < 0) panic("Failed to mount initrd");

	printk(KLOG_INFO "mounted initrd\n");

	asm volatile ("sti");

	r = proc_exec("/init", (const char**){ NULL });
	if (r < 0) panic("Failed to execute init program");

	while (1);
}
