/** \file arch/x86/include/newland/arch/irq.h
 * \author Tristan Ross
 * \copyright GNU Public License 3.0
 * \brief x86 interrupt request
 * \since v0.1.0
 */
#pragma once

#include <newland/arch/idt.h>
#include <sys/types.h>

/**
 * IRQ handler
 */
typedef void (*irq_t)(regs_t* regs);

/**
 * Disables interrupts
 *
 * @return Flags to restore
 */
uint32_t irq_disable();

/**
 * Restores interrupts
 *
 * @param[in] flags Flags to restore
 */
void irq_restore(uint32_t flags);

/**
 * Registers an IRQ handler
 *
 * @param[in] i The IRQ
 * @param[in] handler The function to use for handling
 */
void register_irq_handler(uint8_t i, irq_t handler);
