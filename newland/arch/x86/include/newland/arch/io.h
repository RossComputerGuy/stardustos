/** \file arch/x86/include/newland/arch/io.h
 * \author Tristan Ross
 * \copyright GNU Public License 3.0
 * \brief x86 I/O
 * \since v0.1.0
 */
#pragma once

#include <sys/types.h>

/**
 * Wait for the I/O
 */
#define io_wait() asm volatile ("outb %%al, $0x80" : : "a"(0))

#define DECL_PORT(n, s) void out## n (uint16_t port, uint## s ##_t value); uint## s ##_t in## n(uint16_t port);

DECL_PORT(b, 8)
DECL_PORT(w, 16)
DECL_PORT(l, 32)

#undef DECL_PORT