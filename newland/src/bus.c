/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/bus.h>
#include <newland/error.h>
#include <newland/kalloc.h>
#include <newland/log.h>

SLIST_HEAD(bus_list, bus_t);
static struct bus_list buses = { NULL };
static size_t buses_count = 0;

size_t bus_count() {
  return buses_count;
}

bus_t* bus_get(size_t i) {
  bus_t* bus = NULL;
  size_t index = 0;
  SLIST_FOREACH(bus, &buses, bus_list) {
    if (index == i) return bus;
    index++;
  }
  return NULL;
}

bus_t* bus_fromname(const char* name) {
  bus_t* bus = NULL;
  SLIST_FOREACH(bus, &buses, bus_list) {
   if (!strcmp(bus->name, name)) return bus;
  }
  return NULL;
}

device_t* bus_getdev(bus_t* bus, size_t i) {
  device_t* dev = NULL;
  size_t index = 0;
  SLIST_FOREACH(dev, &bus->dev_list, bus_list) {
    if (index == i) return dev;
    index++;
  }
  return NULL;
}

device_t* bus_getdevbyname(bus_t* bus, const char* name) {
  device_t* dev = NULL;
  SLIST_FOREACH(dev, &bus->dev_list, bus_list) {
    if (!strcmp(dev->name, name)) return dev;
  }
  return NULL;
}

int bus_adddev(bus_t* bus, const char* name) {
  if (strlen(name) > NAME_MAX) return -ENAMETOOLONG;
  device_t* dev = device_fromname(name);
  if (dev == NULL) return -ENOENT;
  if (bus_getdevbyname(bus, dev->name) != NULL) return -EEXIST;
  SLIST_INSERT_HEAD(&bus->dev_list, dev, bus_list);
  bus->dev_count++;
  printk(KLOG_INFO "Attached device (%s) to bus (%s)\n", dev->name, bus->name);
  return 0;
}

int bus_remdev(bus_t* bus, const char* name) {
  if (strlen(name) > NAME_MAX) return -ENAMETOOLONG;
  device_t* dev = device_fromname(name);
  if (dev == NULL) return -ENOENT;
  if (bus_getdevbyname(bus, dev->name) == NULL) return -ENOENT;
  SLIST_REMOVE(&bus->dev_list, dev, device_t, bus_list);
  bus->dev_count--;
  printk(KLOG_INFO "Detached device (%s) to bus (%s)\n", dev->name, bus->name);
  return 0;
}

int register_bus(const char* type, const char* name) {
  if (strlen(type) > NAME_MAX || strlen(name) > NAME_MAX) return -ENAMETOOLONG;
  bus_t* bus = kmalloc(sizeof(bus_t));
  if (bus == NULL) return -ENOMEM;
  strcpy((char*)bus->type, type);
  strcpy((char*)bus->name, name);
  bus->dev_count = 0;
  SLIST_INSERT_HEAD(&buses, bus, bus_list);
  buses_count++;
  printk(KLOG_INFO "Registered bus: %s\n", name);
  return 0;
}

int unregister_bus(const char* name) {
  bus_t* bus = bus_fromname(name);
  if (bus == NULL) return -ENOENT;
  SLIST_REMOVE(&buses, bus, bus_t, bus_list);
  buses_count--;
  kfree(bus);
  printk(KLOG_INFO "Unregistered bus: %s\n", name);
  return 0;
}
