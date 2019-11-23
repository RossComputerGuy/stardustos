/**
 * NewLand Virtual Kernel - (C) 2019 Tristan Ross
 */
#include <libfile/elf.h>
#include <newland/syscall.h>
#include <nvk/emulator.h>
#include <nvk/mem.h>
#include <stdio.h>

static void hook_syscall(uc_engine* uc, void* userdata) {
	int machine = *(int*)userdata;
	switch (machine) {
		case 3:
			{
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
			break;
	}
}

uc_err nvk_emu(void* prog, size_t size, void* impl) {
	uc_engine* uc;
	uc_err err;
	if (prog == NULL) err = uc_open(UC_ARCH_X86, UC_MODE_32, &uc);
	else {
		elf_header_t* hdr = (elf_header_t*)prog;
		switch (hdr->machine) {
			case 3:
				err = uc_open(UC_ARCH_X86, UC_MODE_32, &uc);
				break;
			default: return UC_ERR_ARCH;
		}
	}
	if (err != UC_ERR_OK) {
		fprintf(stderr, "Failed on uc_open() with error returned: %u\n", err);
		return err;
	}
	uc_hook syscall_hook;
	uc_hook_add(uc, &syscall_hook, UC_HOOK_INSN, hook_syscall, NULL, 1, 0, UC_X86_INS_SYSCALL);
	if ((err = uc_mem_map(uc, NVK_RAM_START, NVK_RAM_END, UC_PROT_ALL)) != UC_ERR_OK) {
		fprintf(stderr, "Failed on uc_mem_map() with error returned: %u\n", err);
		uc_close(uc);
		return err;
	}
	if (prog != NULL) {
		elf_header_t* hdr = (elf_header_t*)prog;
		uint32_t offset = NVK_PROG_START;
		uint32_t entry = 0;
		uint32_t entrysize = 0;
		for (int i = 0; i < hdr->phnum; i++) {
			elf_program_t* prg = (elf_program_t*)((uintptr_t*)prog + hdr->phoff + (hdr->phentsize * i));
			if (prg->memsz == 0) continue;
			if ((err = uc_mem_map_ptr(uc, offset, prg->memsz, ((prg->flags & ELF_PROG_READ) ? UC_PROT_READ : 0)
				| ((prg->flags & ELF_PROG_WRITE) ? UC_PROT_WRITE : 0)
				| ((prg->flags & ELF_PROG_EXEC) ? UC_PROT_EXEC : 0), (void*)(prg + prg->offset))) != UC_ERR_OK) {
				fprintf(stderr, "Failed on uc_mem_map_ptr() with error returned: %u\n", err);
				uc_close(uc);
				return err;
			}
			if (prg->memsz >= hdr->entry) entry = hdr->entry + offset;
		}
		for (int i = 0; i < hdr->shnum; i++) {
			elf_section_t* sec = (elf_section_t*)((uintptr_t*)prog + hdr->shoff + (hdr->shentsize * i));
			if (sec->type != ELFS_SYMTAB) continue;
			for (int x = 0; x < (sec->size / sizeof(elf_sym_t)); x++) {
				elf_sym_t* sym = (elf_sym_t*)((uintptr_t*)prog + sec->offset + (sizeof(elf_sym_t) * i));
				if (sym->value == hdr->entry) {
					entrysize = sym->size;
					break;
				}
			}
			if (entrysize != 0) break;
		}
		uc_reg_write(uc, UC_X86_REG_EAX, &impl);
		if ((err = uc_emu_start(uc, entry, entrysize - 1, 0, 0)) != UC_ERR_OK) {
			fprintf(stderr, "Failed on uc_emu_start() with error returned: %u\n", err);
			uc_close(uc);
			return err;
		}
	} else {
		if ((err = uc_mem_map_ptr(uc, NVK_PROG_START, size, UC_PROT_ALL, prog)) != UC_ERR_OK) {
			fprintf(stderr, "Failed on uc_mem_map_ptr() with error returned: %u\n", err);
			uc_close(uc);
			return err;
		}
		uc_reg_write(uc, UC_X86_REG_EAX, &impl);
		if ((err = uc_emu_start(uc, NVK_PROG_START, NVK_PROG_START + size - 1, 0, 0)) != UC_ERR_OK) {
			fprintf(stderr, "Failed on uc_emu_start() with error returned: %u\n", err);
			uc_close(uc);
			return err;
		}
	}
	uc_close(uc);
	return UC_ERR_OK;
}