/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#pragma once

#include <newland/types.h>

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

typedef void (*isr_t)(regs_t regs);

void register_int_handler(uint8_t i, isr_t handler);
void idt_init();
