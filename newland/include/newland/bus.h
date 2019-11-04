/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#pragma once

#include <newland/dev.h>
#include <newland/limits.h>
#include <newland/list.h>
#include <string.h>

SLIST_HEAD(dev_list, device_t);

typedef struct bus {
  SLIST_ENTRY(struct bus) bus_list;
  const char name[NAME_MAX];
  const char type[NAME_MAX];
  struct dev_list dev_list;
  size_t dev_count;
} bus_t;

size_t bus_count();
bus_t* bus_get(size_t i);
bus_t* bus_fromname(const char* name);

device_t* bus_getdev(bus_t* bus, size_t i);
device_t* bus_getdevbyname(bus_t* bus, const char* name);
int bus_adddev(bus_t* bus, const char* name);
int bus_remdev(bus_t* bus, const char* name);

int register_bus(const char* type, const char* name);
int unregister_bus(const char* name);
