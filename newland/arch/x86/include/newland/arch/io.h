/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#pragma once

#include <newland/types.h>

#define io_wait() asm volatile ("outb %%al, $0x80" : : "a"(0))

#define outb(port, val) asm volatile ("outb %0, %1" : : "a"((uint16_t)(val)), "Nd"((uint8_t)(port)))

uint8_t inb(uint16_t port);