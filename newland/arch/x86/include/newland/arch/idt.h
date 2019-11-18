/** \file arch/x86/include/newland/arch/idt.h
 * \author Tristan Ross
 * \copyright GNU Public License 3.0
 * \brief x86 interrupt descriptor table
 * \since v0.1.0
 */
#pragma once

#include <stdint.h>

typedef struct {
	uint32_t ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uint32_t eip, cs, eflags, useresp, ss;
} regs_t;

typedef struct {
	uint16_t base_low;
	uint16_t selector;
	uint8_t always0;
	uint8_t flags;
	uint16_t base_high;
} idt_entry_t;

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idt_t;

/**
 * ISR handler
 */
typedef void (*isr_t)(regs_t* regs);

/**
 * Register an interrupt handler
 *
 * @param[in] i The number of the interrupt
 * @param[in] handler The function to use as the handler
 */
void register_int_handler(uint8_t i, isr_t handler);

/**
 * Initialize the IDT
 */
void idt_init();