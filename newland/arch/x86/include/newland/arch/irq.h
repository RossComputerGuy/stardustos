/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#pragma once

#include <newland/types.h>

uint32_t irq_disable();
void irq_restore(uint32_t flags);