/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/arch/irq.h>

uint32_t irq_disable() {
  uint32_t flags;
  asm volatile ("pushf ; \
  cli ; \
  pop %0" : "=r"(flags) : : "memory");
}

void irq_restore(uint32_t flags) {
  asm ("push %0\n\tpopf" : : "rm"(flags) : "memory", "cc");
}