/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#pragma once

#include <newland/types.h>

typedef struct {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;
  uint8_t granularity;
  uint8_t base_high;
} gdt_entry_t;

typedef struct {
  uint16_t limit;
  uint32_t base;
} gdt_t;

void gdt_init();