/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
.globl paging_invalidate_tlb
paging_invalidate_tlb:
  mov %cr3, %eax
  mov %eax, %cr3
  ret
