/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#pragma once

#include <newland/arch/idt.h>
#include <sys/types.h>

typedef uint32_t (*irq_t)(uint32_t esp, regs_t* regs);

uint32_t irq_disable();
void irq_restore(uint32_t flags);

void register_irq_handler(uint8_t i, irq_t handler);
