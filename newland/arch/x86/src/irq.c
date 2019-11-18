/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/arch/io.h>
#include <newland/arch/irq.h>

static irq_t irq_handlers[16];

uint32_t irq_disable() {
	uint32_t flags;
	asm volatile ("pushf ; \
	cli ; \
	pop %0" : "=r"(flags) : : "memory");
}

void irq_restore(uint32_t flags) {
	asm ("push %0\n\tpopf" : : "rm"(flags) : "memory", "cc");
}

void register_irq_handler(uint8_t i, irq_t handler) {
	irq_handlers[i] = handler;
}

void irq_handler(regs_t* regs) {
	if (regs->int_no >= 40) outb(0xA0, 0x20);
	outb(0x20, 0x20);
	if (irq_handlers[regs->int_no - 32] != 0) irq_handlers[regs->int_no - 32](regs);
}
