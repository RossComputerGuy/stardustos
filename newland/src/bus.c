/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/alloc.h>
#include <newland/bus.h>
#include <newland/log.h>
#include <newland/errno.h>

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

int bus_regdev(bus_t* bus, const char* name, const char* dname) {
	if (bus_getdev(bus, dname) != NULL) return -NEWLAND_EEXIST;
	bus_dev_t* bdev = bus_getdevbyname(bus, name);
	if (bdev == NULL) return -NEWLAND_ENOENT;
	strcpy((char*)bdev->dname, dname);
	// TODO: maybe the bus should have a function call for handling the registration
	return 0;
}

int bus_unregdev(bus_t* bus, const char* name, const char* dname) {
	if (bus_getdev(bus, dname) == NULL) return -NEWLAND_ENOENT;
	bus_dev_t* bdev = bus_getdevbyname(bus, name);
	if (bdev == NULL) return -NEWLAND_ENOENT;
	memset((void*)bdev->dname, 0, NAME_MAX);
	// TODO: maybe the bus should have a function call for handling the unregistring
	return 0;
}

device_t* bus_getdev(bus_t* bus, const char* name) {
	device_t* dev = NULL;
	bus_dev_t* bdev = NULL;
	SLIST_FOREACH(bdev, &bus->dev_list, dev_list) {
		if (strlen(bdev->dname) == 0) continue;
		dev = device_fromname(bdev->dname);
		if (dev == NULL) continue;
		if (!strcmp(dev->name, name)) return dev;
	}
	return NULL;
}

bus_dev_t* bus_getdevbyname(bus_t* bus, const char* name) {
	bus_dev_t* dev = NULL;
	SLIST_FOREACH(dev, &bus->dev_list, dev_list) {
		if (!strcmp(dev->name, name)) return dev;
	}
	return NULL;
}

int bus_adddev(bus_t* bus, const char* name) {
	if (bus_getdevbyname(bus, name) != NULL) return -NEWLAND_EEXIST;
	bus_dev_t* dev = kmalloc(sizeof(bus_dev_t));
	if (dev == NULL) return -NEWLAND_ENOMEM;
	strcpy((char*)dev->name, name);
	SLIST_INSERT_HEAD(&bus->dev_list, dev, dev_list);
	bus->dev_count++;
	return 0;
}

int bus_remdev(bus_t* bus, const char* name) {
	bus_dev_t* dev = bus_getdevbyname(bus, name);
	if (dev == NULL) return -NEWLAND_ENOENT;
	SLIST_REMOVE(&bus->dev_list, dev, bus_dev_t, dev_list);
	bus->dev_count--;
	return 0;
}

int register_bus(const char* type, const char* name) {
	if ((type != NULL && strlen(type) > NAME_MAX) || strlen(name) > NAME_MAX) return -NEWLAND_ENAMETOOLONG;
	bus_t* bus = kmalloc(sizeof(bus_t));
	if (bus == NULL) return -NEWLAND_ENOMEM;
	if (type != NULL) strcpy((char*)bus->type, type);
	strcpy((char*)bus->name, name);
	bus->dev_count = 0;
	SLIST_INSERT_HEAD(&buses, bus, bus_list);
	buses_count++;
	printk(KLOG_INFO "bus: registered: %s\n", name);
	return 0;
}

int unregister_bus(const char* name) {
	bus_t* bus = bus_fromname(name);
	if (bus == NULL) return -NEWLAND_ENOENT;
	SLIST_REMOVE(&buses, bus, bus_t, bus_list);
	buses_count--;
	bus_dev_t* dev = NULL;
	SLIST_FOREACH(dev, &bus->dev_list, dev_list) {
		bus_dev_res_t* res = NULL;
		SLIST_FOREACH(res, &dev->res_list, res_list) {
			kfree(res);
		}
		kfree(dev);
	}
	kfree(bus);
	printk(KLOG_INFO "bus: unregistered: %s\n", name);
	return 0;
}
