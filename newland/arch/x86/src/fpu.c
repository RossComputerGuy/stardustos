/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/arch/fpu.h>
#include <string.h>

char fpu_regs[512] __attribute__((aligned(16)));

void fpu_savectx(proc_t* proc) {
	asm volatile ("fxsave (%0)" :: "r"(fpu_regs));
	memcpy(&proc->fpu_regs, &fpu_regs, 512);
}

void fpu_loadctx(proc_t* proc) {
	memcpy(&fpu_regs, &proc->fpu_regs, 512);
	asm volatile ("fxrstor (%0)" :: "r"(fpu_regs));
}

void fpu_init() {
	asm volatile ("clts");
	size_t t;
	asm volatile("mov %%cr0, %0" : "=r"(t));
	t &= ~(1 << 2);
	t |= (1 << 1);
	asm volatile("mov %0, %%cr0" :: "r"(t));
	asm volatile("mov %%cr4, %0" : "=r"(t));
	t |= 3 << 9;
	asm volatile("mov %0, %%cr4" :: "r"(t));
	asm volatile("fninit");
}
