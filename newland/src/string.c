/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <string.h>

void itoa(char* buf, int base, int d) {
  char* p = buf;
  char* p1, *p2;
  unsigned long ud = d;
  int divisor = 10;

  /* If %d is specified and D is minus, put `-' in the head.  */
  if (base == 'd' && d < 0) {
    *p++ = '-';
    buf++;
    ud = -d;
  } else if (base == 'x') divisor = 16;

  /* Divide UD by DIVISOR until UD == 0.  */
  do {
    int remainder = ud % divisor;
    *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
  } while (ud /= divisor);

  /* Terminate BUF.  */
  *p = 0;

  /* Reverse BUF.  */
  p1 = buf;
  p2 = p - 1;
  while (p1 < p2) {
    char tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    p1++;
    p2--;
  }
}

int strcmp(const char* s1, const char* s2) {
  while (*s1 != '\0' && *s1 == *s2) {
    s1++;
    s2++;
  }
  return (*(unsigned char*) s1) - (*(unsigned char*) s2);
}

char* strcpy(char* dst, const char* src) {
  char* s = dst;
  while (*dst++ = *src++);
  return s;
}

size_t strlen(const char* str) {
  size_t len = 0;
  while (*str++ != '\0') len++;
  return len;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  if (n == 0) return 0;
  while (n-- != 0 && *s1 == *s2) {
    if (n == 0 || *s1 == '\0') break;
    s1++;
    s2++;
  }
  return (*(unsigned char*)s1) - (*(unsigned char*)s2);
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

void* memmove(void* dst, const void* src, size_t size) {
	unsigned char* dstbuff = (unsigned char*)dst;
	const unsigned char* srcbuff = (const unsigned char*)src;
	if (dstbuff < srcbuff) {
		for (size_t i = 0; i < size; i++) dstbuff[i] = srcbuff[i];
	} else {
		for (size_t i = size; i != 0; i--) dstbuff[i-1] = srcbuff[i-1];
	}
	return dstbuff;
}

void* memset(void* ptr, int val, size_t size) {
  unsigned char* buff = (unsigned char*)ptr;
  for (size_t i = 0; i < size; i++) buff[i] = val;
  return ptr;
}

char* ultoa(unsigned long value, char* ptr, int base) {
  unsigned long t = 0, res = 0;
  unsigned long tmp = value;
  int count = 0;
  if (ptr == NULL) return NULL;
  if (tmp == 0) count++;
  while (tmp > 0) {
    tmp = tmp/base;
    count++;
  }
  ptr += count;
  *ptr = '\0';
  do {
    res = value - base * (t = value / base);
    if (res < 10) * -- ptr = '0' + res;
    else if ((res >= 10) && (res < 16)) * --ptr = 'A' - 10 + res;
  } while ((value = t) != 0);
  return ptr;
}