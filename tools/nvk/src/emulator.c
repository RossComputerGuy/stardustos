/**
 * NewLand Virtual Kernel - (C) 2019 Tristan Ross
 */
#include <newland/syscall.h>
#include <nvk/emulator.h>
#include <nvk/mem.h>
#include <stdio.h>

static void hook_syscall(uc_engine* uc, void* userdata) {
/** Read **/
	uint32_t eax;
	uc_reg_read(uc, UC_X86_REG_EAX, &eax);
	uint32_t edi;
	uc_reg_read(uc, UC_X86_REG_EDI, &edi);
	uint32_t esi;
	uc_reg_read(uc, UC_X86_REG_ESI, &esi);
	uint32_t edx;
	uc_reg_read(uc, UC_X86_REG_EDX, &edx);
	uint32_t ecx;
	uc_reg_read(uc, UC_X86_REG_ECX, &ecx);
	uint32_t ebx;
	uc_reg_read(uc, UC_X86_REG_EBX, &ebx);
/** Execute **/
	eax = syscall_run(eax, &edi, &esi, &edx, &ecx, &ebx);
/** Save **/
	uc_reg_write(uc, UC_X86_REG_EAX, &eax);
}

uc_err nvk_emu(void* prog, size_t size, void* impl) {
	uc_engine* uc;
	/* TODO: allow different architectures */
	uc_err err = uc_open(UC_ARCH_X86, UC_MODE_32, &uc);
	if (err != UC_ERR_OK) {
		fprintf(stderr, "Failed on uc_open() with error returned: %u\n", err);
		return err;
	}
	uc_hook syscall_hook;
	uc_hook_add(uc, &syscall_hook, UC_HOOK_INSN, hook_syscall, NULL, 1, 0, UC_X86_INS_SYSCALL);
	// TODO: check if prog is an ELF program and memory map it in
	uc_mem_map(uc, NVK_RAM_START, NVK_RAM_END, UC_PROT_ALL);
	uc_mem_map_ptr(uc, NVK_PROG_START, size, UC_PROT_ALL, prog);
	uc_reg_write(uc, UC_X86_REG_EAX, &impl);
	if ((err = uc_emu_start(uc, NVK_PROG_START, NVK_PROG_START + size - 1, 0, 0)) != UC_ERR_OK) {
		fprintf(stderr, "Failed on uc_emu_start() with error returned: %u\n", err);
		uc_close(uc);
		return err;
	}
	uc_close(uc);
	return UC_ERR_OK;
}