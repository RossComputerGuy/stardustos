/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#pragma once

#include <sys/types.h>

#define io_wait() asm volatile ("outb %%al, $0x80" : : "a"(0))

#define DECL_PORT(n, s) void out## n (uint16_t port, uint## s ##_t value); uint## s ##_t in## n(uint16_t port);

DECL_PORT(b, 8)
DECL_PORT(w, 16)
DECL_PORT(l, 32)

#undef DECL_PORT