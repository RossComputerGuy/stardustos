/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/arch/idt.h>
#include <newland/arch/mem.h>
#include <newland/log.h>
#include <newland/errno.h>
#include <stdlib.h>
#include <string.h>

extern uint32_t __kernel_end;

static uint32_t used_mem = 0;
static uint32_t total_mem = 0;
static unsigned char mem[1024 * 1024 / 8] = { 0 };

#define PHYSICAL_IS_USED(__addr) (mem[(unsigned int)(__addr) / PAGE_SIZE / 8] & (1 << ((unsigned int)(__addr) / PAGE_SIZE % 8)))
#define PHYSICAL_SET_USED(__addr) (mem[(unsigned int)(__addr) / PAGE_SIZE / 8] |= (1 << ((unsigned int)(__addr) / PAGE_SIZE % 8)))
#define PHYSICAL_SET_FREE(__addr) (mem[(unsigned int)(__addr) / PAGE_SIZE / 8] &= ~(1 << ((unsigned int)(__addr) / PAGE_SIZE % 8)))
#define free_mem (used_mem - total_mem)
#define phys_free phys_setfree
#define PAGE_ALIGN(__x) ((__x) + PAGE_SIZE - ((__x) % PAGE_SIZE))

int phys_isused(unsigned int addr, unsigned int count) {
	for (unsigned int i = 0; i < count; i++) {
		if (PHYSICAL_IS_USED(addr + (i * PAGE_SIZE))) return 1;
	}
	return 0;
}

void phys_setused(unsigned int addr, unsigned int count) {
	for (unsigned int i = 0; i < count; i++) {
		if (!PHYSICAL_IS_USED(addr + (i * PAGE_SIZE))) {
			used_mem += PAGE_SIZE;
			PHYSICAL_SET_USED(addr + (i * PAGE_SIZE));
		}
	}
}

void phys_setfree(unsigned int addr, unsigned int count) {
	for (unsigned int i = 0; i < count; i++) {
		if (PHYSICAL_IS_USED(addr + (i * PAGE_SIZE))) {
			used_mem -= PAGE_SIZE;
			PHYSICAL_SET_FREE(addr + (i * PAGE_SIZE));
		}
	}
}

unsigned int phys_alloc(unsigned int count) {
	for (unsigned int i = 0; i < (total_mem / PAGE_SIZE); i++) {
		unsigned int addr = i * PAGE_SIZE;
		if (!phys_isused(addr, count)) {
			phys_setused(addr, count);
			return addr;
		}
	}
	return -NEWLAND_ENOMEM;
}

/** Virtual Memory Management **/

#define PD_INDEX(vaddr) ((vaddr) >> 22)
#define PT_INDEX(vaddr) (((vaddr) >> 12) & 0x03ff)

static page_dir_t krnl_pgdir __attribute__((aligned(PAGE_SIZE))) = { 0 };
static page_table_t krnl_pgtbl[256] __attribute__((aligned(PAGE_SIZE))) = { 0 };

int page_ispresent(page_dir_t* dir, unsigned int vaddr) {
	unsigned int pdi = PD_INDEX(vaddr);
	unsigned int pti = PT_INDEX(vaddr);
	page_dir_entry_t* pde = &dir->entries[pdi];
	if (pde->present == 0) return 0;
	page_table_t* pt = (page_table_t*)(pde->frame * PAGE_SIZE);
	page_t* pg = &pt->entries[pti];
	return pg->present;
}

int virt_ispresent(page_dir_t* dir, unsigned int vaddr, unsigned int count) {
	for (unsigned int i = 0; i < count; i++) {
		unsigned int offset = i * PAGE_SIZE;
		if (!page_ispresent(dir, vaddr + offset)) return 0;
	}
	return 1;
}

unsigned int virt2phys(page_dir_t* dir, unsigned int vaddr) {
	unsigned int pdi = PD_INDEX(vaddr);
	unsigned int pti = PT_INDEX(vaddr);
	page_dir_entry_t* pde = &dir->entries[pdi];
	page_table_t* pt = (page_table_t*)(pde->frame * PAGE_SIZE);
	page_t* pg = &pt->entries[pti];
	return (pg->frame * PAGE_SIZE) + (vaddr & 0xfff);
}

int virt_map(page_dir_t* dir, unsigned int vaddr, unsigned int paddr, unsigned int count, int iswrite, int isuser) {
	for (unsigned int i = 0; i < count; i++) {
		unsigned int offset = i * PAGE_SIZE;
		unsigned int pdi = PD_INDEX(vaddr + offset);
		unsigned int pti = PT_INDEX(vaddr + offset);
		page_dir_entry_t* pde = &dir->entries[pdi];
		page_table_t* pt = (page_table_t*)(pde->frame * PAGE_SIZE);
		if (pde->present == 0) {
			pt = (page_table_t*)mem_allocident(dir, 1, iswrite, 0);
			pde->present = 1;
			pde->write = iswrite;
			pde->user = isuser;
			pde->frame = (uint32_t)(pde) >> 12;
		}
		page_t* pg = &pt->entries[pti];
		pg->present = 1;
		pg->write = iswrite;
		pg->user = isuser;
		pg->frame = (paddr + offset) >> 12;
	}
	paging_invalidate_tlb();
	return 0;
}

void virt_unmap(page_dir_t* dir, unsigned int vaddr, unsigned int count) {
	for (unsigned int i = 0; i < count; i++) {
		unsigned int offset = i * PAGE_SIZE;
		unsigned int pdi = PD_INDEX(vaddr + offset);
		unsigned int pti = PT_INDEX(vaddr + offset);
		page_dir_entry_t* pde = &dir->entries[pdi];
		page_table_t* pt = (page_table_t*)(pde->frame * PAGE_SIZE);
		page_t* pg = &pt->entries[pti];
		if (pg->present) pg->value = 0;
	}
	paging_invalidate_tlb();
}

unsigned int virt_alloc(page_dir_t* dir, unsigned int paddr, unsigned int count, int iswrite, int isuser) {
	if (count == 0) return 0;
	unsigned int curr_size = 0;
	unsigned int startaddr = 0;
	for (size_t i = (isuser ? 256 : 0) * 1024; i < (isuser ? 1024 : 256) * 1024; i++) {
		unsigned int vaddr = i * PAGE_SIZE;
		if (!page_ispresent(dir, vaddr)) {
			if (curr_size == 0) startaddr = vaddr;
			curr_size++;
			if (curr_size == count) {
				virt_map(dir, startaddr, paddr, count, iswrite, isuser);
				return startaddr;
			}
		} else curr_size = 0;
	}
	return 0;
}

void mem_loadmmap(multiboot_info_t* mbi) {
	total_mem = 0;
	for (multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)mbi->mmap_addr; (uint32_t)mmap < mbi->mmap_addr + mbi->mmap_length; mmap = (multiboot_memory_map_t*)((uint32_t)mmap + mmap->size + sizeof(mmap->size))) {
		if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) total_mem += mmap->len;
		else {
			for (uint64_t addr = mmap->addr; addr < mmap->len; addr += PAGE_SIZE) {
				PHYSICAL_SET_USED(addr);
			}
		}
	}
	multiboot_module_t* mod = (multiboot_module_t*)mbi->mods_addr;
	for (size_t i = 0; i < mbi->mods_count; i++) mod = (multiboot_module_t*)((uint32_t)mod->mod_end + 1);
	used_mem = PAGE_ALIGN(max((uint32_t)&__kernel_end, (uint32_t)mod));
	printk(KLOG_INFO "mem: memory map loaded (t: %dKib, u: %dKib)\n", total_mem / 1024, used_mem / 1024);
}

void mem_init(multiboot_info_t* mbi) {
	memset(mem, 0, sizeof(mem));

	for (unsigned int i = 0; i < 256; i++) {
		page_dir_entry_t* pde = &krnl_pgdir.entries[i];
		pde->user = 0;
		pde->write = 1;
		pde->present = 1;
		pde->frame = (unsigned int)&krnl_pgtbl[i] / PAGE_SIZE;
	}

	mem_loadmmap(mbi);
	mem_identmap(&krnl_pgdir, 0, PAGE_ALIGN(used_mem) / PAGE_SIZE + 1);
	paging_loaddir(&krnl_pgdir);
	paging_enable();
}

unsigned int mem_allocident(page_dir_t* dir, unsigned int count, int iswrite, int isuser) {
	if (count == 0) return 0;
	unsigned int curr_size = 0;
	unsigned int startaddr = 0;
	for (size_t i = (isuser ? 256 : 0); i < (isuser ? 1024 : 256) * 1024; i++) {
		unsigned int addr = i * PAGE_SIZE;
		if (!(page_ispresent(dir, addr) || phys_isused(addr, 1))) {
			if (curr_size == 0) startaddr = addr;
			curr_size++;
			if (curr_size == count) {
				phys_setused(startaddr, count);
				virt_map(dir, startaddr, startaddr, count, iswrite, isuser);
				memset((void*)startaddr, 0, count * PAGE_SIZE);
				return startaddr;
			}
		} else curr_size = 0;
	}
	return 0;
}

unsigned int mem_alloc(page_dir_t* dir, unsigned int count, int iswrite, int isuser) {
	if (count == 0) return 0;
	unsigned int paddr = phys_alloc(count);
	if (paddr == 0) return 0;
	unsigned int vaddr = virt_alloc(dir, paddr, count, iswrite, isuser);
	if (vaddr == 0);
	memset((void*)vaddr, 0, count * PAGE_SIZE);
	return vaddr;
}

void mem_free(page_dir_t* dir, unsigned int addr, unsigned int count) {
	if (virt_ispresent(dir, addr, count)) {
		phys_free(virt2phys(dir, addr), count);
		virt_unmap(dir, addr, count);
	}
}

int mem_identmap(page_dir_t* dir, unsigned int addr, unsigned int count) {
	phys_setused(addr, count);
	virt_map(dir, addr, addr, count, 1, 0);
	return 0;
}

int mem_map(page_dir_t* pgdir, unsigned int addr, unsigned int count, int iswrite, int isuser) {
	for (unsigned int i = 0; i < count; i++) {
		unsigned int vaddr = addr + i * PAGE_SIZE;
		if (!virt_ispresent(pgdir, vaddr, 1)) {
			unsigned int paddr = phys_alloc(1);
			virt_map(pgdir, vaddr, paddr, 1, iswrite, isuser);
		}
	}
	return 0;
}

page_dir_t* get_krnlpgdir() {
	return &krnl_pgdir;
}

page_dir_t* mem_alloc_pgdir() {
	page_dir_t* pgdir = (page_dir_t*)mem_allocident(&krnl_pgdir, 1, 1, 0);
	if (pgdir == NULL) return NULL;
	memset(pgdir, 0, sizeof(page_dir_t));
	for (unsigned int i = 0; i < 256; i++) {
		page_dir_entry_t* pde = &pgdir->entries[i];
		pde->user = 0;
		pde->write = 1;
		pde->present = 1;
		pde->frame = (unsigned int)&krnl_pgtbl[i] / PAGE_SIZE;
	}
	return pgdir;
}

void mem_free_pgdir(page_dir_t* dir) {
	for (size_t i = 256; i < 1024; i++) {
		page_dir_entry_t* pde = &dir->entries[i];
		if (pde->present) {
			page_table_t* pt = (page_table_t*)(pde->frame * PAGE_SIZE);
			for (size_t x = 0; i < 1024; i++) {
				page_t* pg = &pt->entries[x];
				if (pg->present) phys_free(pg->frame * PAGE_SIZE, 1);
			}
			mem_free(&krnl_pgdir, (unsigned int)pt, 1);
		}
	}
	mem_free(&krnl_pgdir, (unsigned int)dir, 1);
}
