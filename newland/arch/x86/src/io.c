/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/arch/io.h>

#define DEF_PORT(n, s) void out## n (uint16_t port, uint## s ##_t value) { asm volatile ("out"#n" %0, %1" :: "a"(value), "Nd"(port)); } \
	uint## s ##_t in## n (uint16_t port) { uint## s ##_t ret; asm volatile ("in"#n" %1, %0" : "=a"(ret) : "Nd"(port)); return ret; }

DEF_PORT(b, 8)
DEF_PORT(w, 16)
DEF_PORT(l, 32)