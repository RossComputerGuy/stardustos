/**
 * NewLand Virtual Kernel - (C) 2019 Tristan Ross
 */
#include <nvk/emulator.h>
#include <nvk/mem.h>
#include <stdio.h>

static void hook_syscall(uc_engine* uc, void* userdata) {
	uint32_t eax;
	uc_reg_read(uc, UC_X86_REG_EAX, &eax);
	// TODO: hook into the NewLand kernel's system call executor
}

uc_err nvk_emu(void* prog, size_t size, void* impl) {
	uc_engine* uc;
	/* TODO: allow different architectures */
	uc_err err = uc_open(UC_ARCH_X86, UC_MODE_32, &uc);
	if (err != UC_ERR_OK) {
		fprintf(stderr, "Failed on uc_open() with error returned: %u\n", err);
		return err;
	}
	uc_mem_map(uc, NVK_RAM_START, NVK_RAM_END, UC_PROT_ALL);
	uc_mem_map_ptr(uc, NVK_PROG_START, size, UC_PROT_ALL, prog);
	uc_reg_write(uc, UC_X86_REG_EAX, &impl);
	if ((err = uc_emu_start(uc, NVK_PROG_START, NVK_PROG_START + size - 1, 0, 0)) != UC_ERR_OK) {
		fprintf(stderr, "Failed on uc_open() with error returned: %u\n", err);
		uc_close(uc);
		return err;
	}
	uc_close(uc);
	return UC_ERR_OK;
}