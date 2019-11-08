/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/error.h>
#include <newland/limits.h>
#include <newland/log.h>
#include <newland/time.h>
#include <newland-config.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define TIME_STRLEN 12

static char klog_buffer[NEWLAND_KLOG_SIZE] = { 0 };
static size_t klog_pos = 0;

extern void arch_logc(const char c);

static int getlevel(const char* msg) {
  if (msg[0] == KLOG_SOH_ASCII && msg[1] >= 0 && msg[1] < 7) return msg[1];
  return KLEVEL_NOTICE;
}

static int print(const char* str, size_t len, int inmsg) {
  int level = getlevel(str);
  if (str[0] == KLOG_SOH_ASCII) {
    str += 2;
    len -= 2;
  }
  size_t freesize = NEWLAND_KLOG_SIZE - klog_pos;
  if (!inmsg) len += TIME_STRLEN + 3;
  if (freesize < len) {
    size_t l = 0;
    while (klog_buffer[l] != '\n') l++;
    memmove(klog_buffer, klog_buffer + l, NEWLAND_KLOG_SIZE - l);
    klog_pos -= l;
  }
  char* buff = klog_buffer + klog_pos;
  if (!inmsg) {
    buff[0] = '[';
    memset(buff + 1, ' ', TIME_STRLEN);
    time_t t = boottime(NULL);
    t /= 10000.0;
    ultoa(t, buff + 1, 10);
    buff += 1 + TIME_STRLEN;
    buff[0] = ']';
    buff[1] = ' ';
    buff += 2;
  } else buff++;
  strcpy(buff, str);
  buff[len] = 0;
  buff = klog_buffer + klog_pos;
  for (size_t i = 0; i < len + 1; i++) {
    arch_logc(buff[i]);
  }
  klog_pos += len;
  return len;
}

int putk(const char* str) {
  return print(str, strlen(str), 0);
}

int printk(const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  char buff[512];
  int wrote = vsnprintf(buff, sizeof(buff), fmt, ap);
  if (wrote < 0) {
    va_end(ap);
    return wrote;
  }
  wrote = print(buff, strlen(buff), 0);
  va_end(ap);
  return wrote;
}
