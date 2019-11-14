/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
.globl paging_invalidate_tlb
paging_invalidate_tlb:
	movl %cr3, %eax
	movl %eax, %cr3
	ret

.globl paging_enable
paging_enable:
	movl %cr0, %eax
	orl $0x80000000, %eax
	movl %eax, %cr0
	ret

.globl paging_disable
paging_disable:
	movl %cr0, %eax
	orl $0x7fffffff, %eax
	movl %eax, %cr0
	ret

.globl paging_loaddir
paging_loaddir:
	movl 4(%esp), %eax
	movl %eax, %cr3
	ret
