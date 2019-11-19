/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/alloc.h>
#include <newland/dev.h>
#include <newland/errno.h>

SLIST_HEAD(dev_list, device_t);
static struct dev_list devices = { NULL };
static size_t dev_count = 0;

size_t device_count() {
	return dev_count;
}

size_t device_indexof(device_t* device) {
	device_t* dev = NULL;
	size_t index = 0;
	SLIST_FOREACH(dev, &devices, dev_list) {
		if (!strcmp(dev->name, device->name)) return index;
		index++;
	}
	return -1;
}

device_t* device_get(size_t i) {
	device_t* device = NULL;
	size_t index = 0;
	SLIST_FOREACH(device, &devices, dev_list) {
		if (index == i) return device;
		index++;
	}
	return NULL;
}

device_t* device_fromdev(dev_t dev) {
	device_t* device = NULL;
	SLIST_FOREACH(device, &devices, dev_list) {
		if (device->dev == dev) return device;
	}
	return NULL;
}

device_t* device_fromname(const char* name) {
	if (strlen(name) > NAME_MAX) return NULL;
	device_t* device = NULL;
	SLIST_FOREACH(device, &devices, dev_list) {
		if (!strcmp(device->name, name)) return device;
	}
	return NULL;
}

int register_device(dev_t dev, const char* name, fs_node_opts_t opts, size_t size) {
	if (strlen(name) > NAME_MAX) return -NEWLAND_ENAMETOOLONG;
	if (device_fromname(name) != NULL || device_fromdev(dev) != NULL) return -NEWLAND_EEXIST;
	device_t* device = kmalloc(sizeof(device_t));
	if (device == NULL) return -NEWLAND_ENOMEM;
	device->dev = dev;
	device->size = size;
	strcpy((char*)device->name, name);
	device->opts = opts;
	SLIST_INSERT_HEAD(&devices, device, dev_list);
	dev_count++;
	return 0;
}

int unregister_device(dev_t dev) {
	device_t* device = device_fromdev(dev);
	if (device == NULL) return -NEWLAND_ENODEV;
	SLIST_REMOVE(&devices, device, device_t, dev_list);
	dev_count--;
	kfree(device);
	return 0;
}
