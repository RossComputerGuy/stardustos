/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
.globl gdt_reloadseg
gdt_reloadseg:
	jmp $0x08, $.flush
.flush:
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	ret

.globl tss_flush
tss_flush:
	mov $0x2B, %ax
	ltr %ax
	ret
