/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#pragma once

#include <newland/dev.h>
#include <newland/limits.h>
#include <liblist.h>
#include <string.h>

#define BUSDEVRES_IO 1
#define BUSDEVRES_MEM 2

typedef struct bus_dev_res {
  SLIST_ENTRY(struct bus_dev_res) res_list;
  uint64_t value;
  size_t size;
  int type;
} bus_dev_res_t;

SLIST_HEAD(bus_dev_res_list, bus_dev_res_t);

#define BUSDEV_INT (1 << 0)

typedef struct bus_dev {
  SLIST_ENTRY(struct bus_dev) dev_list;
  const char name[NAME_MAX];
  const char dname[NAME_MAX];

  uint32_t addr;

  struct bus_dev_res_list res_list;
  size_t res_count;

  uint8_t flags;
  uint16_t interrupt;
} bus_dev_t;

SLIST_HEAD(bus_dev_list, bus_dev_t);

typedef struct bus {
  SLIST_ENTRY(struct bus) bus_list;
  const char name[NAME_MAX];
  const char type[NAME_MAX];
  struct bus_dev_list dev_list;
  size_t dev_count;
} bus_t;

size_t bus_count();
bus_t* bus_get(size_t i);
bus_t* bus_fromname(const char* name);

int bus_regdev(bus_t* bus, const char* name, const char* dname);
int bus_unregdev(bus_t* bus, const char* name, const char* dname);
device_t* bus_getdev(bus_t* bus, const char* name);
bus_dev_t* bus_getdevbyname(bus_t* bus, const char* name);
int bus_adddev(bus_t* bus, const char* name);
int bus_remdev(bus_t* bus, const char* name);

int register_bus(const char* type, const char* name);
int unregister_bus(const char* name);
