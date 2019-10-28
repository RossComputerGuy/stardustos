/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/arch/misc.h>
#include <newland/arch/texcon.h>
#include <string.h>

void halt() {
_halt:
  asm volatile ("hlt");
  goto _halt;
}

void panic(const char* msg) {
  texcon_write(msg, strlen(msg));
  halt();
}