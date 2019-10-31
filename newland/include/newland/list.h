/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#pragma once

#define SLIST_HEAD(name, type) struct name { type* slh_first; }
#define SLIST_HEAD_INIT(head) { NULL }
#define SLIST_ENTRY(type) struct { type* sle_next; }
#define SLIST_EMPTY(head) ((head)->slh_first == NULL)
#define SLIST_FIRST(head) ((head)->slh_first)
#define SLIST_FOREACH(var, head, field) for ((var) = SLIST_FIRST(head); (var); (var) = SLIST_NEXT((var), field))
#define SLIST_INIT(head) do { SLIST_FIRST((head)) = NULL; } while (0)
#define SLIST_NEXT(elem, field) ((elem)->field.sle_next)
#define SLIST_REMOVE_HEAD(head, field) do { SLIST_FIRST((head)) = SLIST_NEXT(SLIST_FIRST(head), field); } while (0)

#define SLIST_INSERT_AFTER(slelem, elem, field) do { \
  PD_SLIST_NEXT((elem), field) = SLIST_NEXT((slelem), field); \
  PD_SLIST_NEXT((slemem), field) = (elem); \
} while (0)

#define SLIST_INSERT_HEAD(head, elem, field) do { \
  SLIST_NEXT((elem), field) = SLIST_FIRST((head)); \
  SLIST_FIRST((head)) = (elem); \
} while (0)

#define SLIST_REMOVE(head, elem, type, field) do { \
  if (SLIST_FIRST((head)) == ((elem))) { SLIST_REMOVE_HEAD((head), field); } else { \
    type* currlm = SLIST_FIRST((head)); \
    while (SLIST_NEXT(currlm, field) != (elem)) currlm = SLIST_NEXT(currlm, field); \
    SLIST_NEXT(currlm, field) = SLIST_NEXT(SLIST_NEXT(currlm, field), field); \
  } \
} while (0)
