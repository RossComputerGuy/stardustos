/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/arch/gdt.h>

static gdt_entry_t gdt_entries[6];
static gdt_t gdt;
static tss_t tss = { .ss0 = 0x10, .esp0 = 0, .cs = 0x0b, .ss = 0x13, .ds = 0x13, .es = 0x13, .fs = 0x13, .gs = 0x13 };

extern void gdt_reloadseg();
extern void tss_flush();

static void gdt_set_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
	gdt_entry_t* entry = &gdt_entries[num];

	entry->base_low = (base & 0xFFFF);
	entry->base_middle = (base >> 16) & 0xFF;
	entry->base_high = (base >> 24) & 0xFF;

	entry->limit_low = (limit & 0xFFFF);
	entry->granularity = ((limit >> 16) & 0x0F) | gran & 0xF0;

	entry->access = access;
}

void gdt_init() {
	gdt.limit = sizeof(gdt_entry_t) * 6 - 1;
	gdt.base = (uint32_t)&gdt_entries;

	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
	gdt_set_gate(5, (uintptr_t)&tss, sizeof(tss_t), 0xE9, 0x0); 

	asm volatile ("lgdtl %0" :: "m"(gdt));
	gdt_reloadseg();

	tss_flush();
}
