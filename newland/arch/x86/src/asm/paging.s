/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
.globl paging_invalidate_tlb
paging_invalidate_tlb:
  mov %cr3, %eax
  mov %eax, %cr3
  ret

.globl paging_enable
paging_enable:
  mov %cr0, %eax
  or $0x80000000, %eax
  mov %eax, %cr0
  ret

.globl paging_disable
paging_disable:
  mov %cr0, %eax
  or $0x7fffffff, %eax
  mov %eax, %cr0
  ret

.globl paging_loaddir
paging_loaddir:
  mov 4(%esp), %eax
  mov %eax, %cr3
  ret
