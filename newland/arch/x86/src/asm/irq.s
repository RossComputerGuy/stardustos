/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
.macro IRQ irqnum, isrnum
	.globl irq\irqnum
	irq\irqnum:
		cli
		push $0
		push $\isrnum
		jmp irq_common_stub
.endm

irq_common_stub:
	pusha
	mov %ds, %ax
	push %eax
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	push %esp
	call irq_handler
	pop %ebx
	pop %ebx
	mov %bx, %ds
	mov %bx, %es
	mov %bx, %fs
	mov %bx, %gs
	popa
	add $8, %esp
	sti
	iret

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47
