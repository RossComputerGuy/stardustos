/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/dev/tty.h>
#include <newland/dev.h>
#include <newland/error.h>
#include <newland/kalloc.h>

SLIST_HEAD(tty_list, tty_t);
static struct tty_list ttys = { NULL };
static size_t tty_cnt = 0;

size_t tty_count() {
  return tty_cnt;
}

tty_t* tty_get(size_t i) {
  tty_t* tty = NULL;
  size_t index = 0;
  SLIST_FOREACH(tty, &ttys, tty_list) {
    if (index == i) return tty;
    index++;
  }
  return NULL;
}

tty_t* tty_fromname(const char* name) {
  tty_t* tty = NULL;
  SLIST_FOREACH(tty, &ttys, tty_list) {
    if (!strcmp(tty->name, name)) return tty;
  }
  return NULL;
}