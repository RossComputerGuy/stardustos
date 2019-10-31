/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <string.h>

size_t strlen(const char* str) {
  size_t len = 0;
  while (*str++ != '\0') len++;
  return len;
}

char* strncpy(char* dst, const char* src, size_t count) {
  char* ds = dst;
  char* ss = (char*)src;
  while (count > 0) {
    count--;
    if ((*ds++ = *ss++) == '\0') break;
  }
  while (count-- > 0) *ds++ = '\0';
  return dst;
}

void* memcpy(void* dst, const void* src, size_t size) {
	unsigned char* buffdst = (unsigned char*)dst;
	const unsigned char* buffsrc = (const unsigned char*)src;
	for (size_t i = 0; i < size; i++) buffdst[i] = buffsrc[i];
	return buffdst;
}

void* memset(void* ptr, int val, size_t size) {
  unsigned char* buff = (unsigned char*)ptr;
  for (size_t i = 0; i < size; i++) buff[i] = val;
  return ptr;
}
