/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/arch/misc.h>
#include <newland/arch/texcon.h>
#include <string.h>

void halt() {
  while (1) asm volatile ("hlt");
}

void panic(const char* msg) {
  texcon_write(msg, strlen(msg));
  halt();
}
