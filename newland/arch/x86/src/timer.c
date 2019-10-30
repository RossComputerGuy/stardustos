/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/arch/io.h>
#include <newland/arch/irq.h>
#include <newland/arch/timer.h>

static void timer_handle(regs_t regs) {
}

void timer_init(uint32_t freq) {
  register_irq_handler(0, timer_handle);
  uint32_t div = 1193180 / freq;
  outb(0x43, 0x36);
  outb(0x40, div & 0xFF);
  outb(0x40, (div >> 8) & 0xFF);
}