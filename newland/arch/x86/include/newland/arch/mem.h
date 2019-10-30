/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#pragma once

#include <newland/types.h>

#define kmalloc_a(sz) kmalloc_int(sz, 0, 0)
#define kmalloc_p(sz, phys) kmalloc_int(sz, 0, phys)
#define kmalloc_ap(sz, phys) kmalloc_int(sz, 1, phys)
#define kmalloc(sz) kmalloc_int(sz, 0, 0)

uint32_t kmalloc_int(uint32_t sz, int align, uint32_t* phys);