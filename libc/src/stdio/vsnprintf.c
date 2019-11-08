/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static char* convert(unsigned int num, int base) {
  static char* rep = "0123456789ABCDEF";
  static char buff[50];
  char* ptr = &buff[49];
  *ptr = '\0';
  do {
    *--ptr = rep[num % base];
    num /= base;
  } while (num != 0);
  return ptr;
}

int vsnprintf(char* str, size_t size, const char* format, va_list ap) {
  int wrote = 0;
  char* traverse =  format;
  unsigned int i = 0;
  char* s = NULL;
  for (; *traverse != '\0'; traverse++) {
    while (*traverse != '%') {
      str[wrote++] = *traverse;
      traverse++;
    }
    traverse++;
    switch (*traverse) {
      case 'c':
        i = va_arg(ap, int);
        str[wrote++] = *traverse;
        break;
      case 'd':
        i = va_arg(ap, int);
        if (i < 0) {
          i = -i;
          str[wrote++] = '-';
        }
        s = convert(i, 10);
        strncpy((char*)(str + wrote), s, strlen(s));
        wrote += strlen(s);
        break;
    }
  }
  return wrote;
}
