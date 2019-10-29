/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <string.h>

size_t strlen(const char* str) {
  size_t len = 0;
  while (*str++ != '\0') len++;
  return len;
}

void* memset(void* ptr, int val, size_t size) {
  unsigned char* buff = (unsigned char*)ptr;
  for (size_t i = 0; i < size; i++) buff[i] = val;
  return ptr;
}