/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/arch/mem.h>

extern uint32_t __kernel_end;
uint32_t placement_address = (uint32_t)&__kernel_end;

uint32_t kmalloc_int(uint32_t sz, int align, uint32_t* phys) {
  if (align == 1 && (placement_address & 0xFFFFF000)) {
    placement_address &= 0xFFFFF000;
    placement_address += 0x1000;
  }
  if (phys) *phys = placement_address;
  uint32_t tmp = placement_address;
  placement_address += sz;
  return tmp;
}