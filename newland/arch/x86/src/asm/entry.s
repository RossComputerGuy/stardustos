/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */

.set ALIGN, (1 << 0)
.set MEMINFO, (1 << 1)
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text
.globl _start
_start:
	movl $stack_top, %esp
	pushl $0
	popf
	pushl %ebx
	pushl %eax
	call bootstrap_main
	cli
1: hlt
	jmp 1b
