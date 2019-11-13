/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
.section .text

.globl _start
_start:
	call __libc_init
	call main
	pushl %eax
	call __libc_fini
