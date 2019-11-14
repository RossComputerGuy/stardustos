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

/**
 * A structure which represents resources such as memory addresses and IO for a device on a bus
 */
typedef struct bus_dev_res {
	SLIST_ENTRY(struct bus_dev_res) res_list;
	uint64_t value;
	size_t size;
	int type;
} bus_dev_res_t;

SLIST_HEAD(bus_dev_res_list, bus_dev_res_t);

#define BUSDEV_INT (1 << 0)

/**
 * Structure that represents a device on a bus
 */
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

/**
 * Structure that represents a bus
 */
typedef struct bus {
	SLIST_ENTRY(struct bus) bus_list;
	const char name[NAME_MAX];
	const char type[NAME_MAX];
	struct bus_dev_list dev_list;
	size_t dev_count;
} bus_t;

/**
 * Retrieves the number of registered buses
 *
 * @return A number
 */
size_t bus_count();

/**
 * Gets a bus by its index in the registered buses list
 *
 * @param[in] i The index of the bus
 * @return A bus or NULL if not found
 */
bus_t* bus_get(size_t i);

/**
 * Gets a registered bus by its name
 *
 * @param[in] name The name of the bus
 * @return A bus or NULL if not found
 */
bus_t* bus_fromname(const char* name);

/**
 * Registers a device driver with a bus device
 *
 * @param[in] bus The bus to use
 * @param[in] name The name of the bus device
 * @param[in] dname The name of the device driver
 * @return Zero on success or a negative errno code
 */
int bus_regdev(bus_t* bus, const char* name, const char* dname);

/**
 * Unregisters a device driver with a bus device
 *
 * @param[in] bus The bus to use
 * @param[in] name The name of the bus device
 * @param[in] dname The name of the device driver
 * @return Zero on success or a negative errno code
 */
int bus_unregdev(bus_t* bus, const char* name, const char* dname);

/**
 * Gets the device driver for a bus device
 *
 * @param[in] bus The bus device to search in
 * @param[in] name The bus device name
 * @return The device driver for a bus device or NULL if none is attached
 */
device_t* bus_getdev(bus_t* bus, const char* name);

/**
 * Gets a bus device by its name
 *
 * @param[in] bus The bus to search in
 * @param[in] name The name of the bus device
 * @return A bus device or NULL if not found
 */
bus_dev_t* bus_getdevbyname(bus_t* bus, const char* name);

/**
 * Adds a device to the bus
 *
 * @param[in] bus The bus to add a device to
 * @param[in] name The name of the bus device to add
 * @return Zero on success or a negative errno code
 */
int bus_adddev(bus_t* bus, const char* name);

/**
 * Removes a device from the bus
 *
 * @param[in] bus The bus to remove its device
 * @param[in] name The name of the bus device to remove
 * @return Zero on success or a negative errno code
 */
int bus_remdev(bus_t* bus, const char* name);

/**
 * Registers a bus
 *
 * @param[in] type The type of bus as a string, this is outlined more in the kernel documentation
 * @param[in] name The name of the bus
 * @return Zero on success or a negative errno code
 */
int register_bus(const char* type, const char* name);

/**
 * Unregisters a bus
 *
 * @param[in] name The name of the registered bus
 * @return Zero on success or a negative errno code
 */
int unregister_bus(const char* name);
