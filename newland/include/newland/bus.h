/** \file include/newland/bus.h
 * \author Tristan Ross
 * \copyright GNU Public License 3.0
 * \brief Bus management and bus driver API
 * \since v0.1.0
 * \details This portion of the NewLand kernel contains code for managing buses and reading them. Detailed explanation can be found in the kernel manual.
 */
#pragma once

#include <newland/dev.h>
#include <liblist.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

/**
 * I/O resource for bus devices
 */
#define BUSDEVRES_IO 1

/**
 * Memory resource for bus devices
 */
#define BUSDEVRES_MEM 2

/**
 * A structure which represents resources such as memory addresses and IO for a device on a bus
 */
typedef struct bus_dev_res {
	SLIST_ENTRY(struct bus_dev_res) res_list;
	/**
	 * The value of the resource, if its a memory resource then its going to be the address
	 */
	uint64_t value;

	/**
	 * The size in bytes the resource takes up
	 */
	size_t size;

	/**
	 * The type of resource, can only be BUSDEVRES_IO or BUSDEVRES_MEM
	 */
	int type;
} bus_dev_res_t;

SLIST_HEAD(bus_dev_res_list, bus_dev_res_t);

/**
 * A flag used for bus devices for telling the kernel there's an interrupt handler that is needed
 */
#define BUSDEV_INT (1 << 0)

/**
 * Structure that represents a device on a bus
 */
typedef struct bus_dev {
	SLIST_ENTRY(struct bus_dev) dev_list;

	/**
	 * The name of the device on the bus
	 */
	const char name[NAME_MAX];

	/**
	 * The name of the device driver that's attached to this bus device
	 */
	const char dname[NAME_MAX];

	/**
	 * The bus device address, should be used for the read and write bus functions
	 */
	uint32_t addr;

	/**
	 * A list of resources the device has
	 */
	struct bus_dev_res_list res_list;

	/**
	 * The number of resources the device has
	 */
	size_t res_count;

	/**
	 * Bus device flags
	 */
	uint8_t flags;

	/**
	 * The interrupt number, should only be set when the interrupt flag is set
	 */
	uint16_t interrupt;
} bus_dev_t;

SLIST_HEAD(bus_dev_list, bus_dev_t);

/**
 * Structure that represents a bus
 */
typedef struct bus {
	SLIST_ENTRY(struct bus) bus_list;

	/**
	 * The name of the bus
	 */
	const char name[NAME_MAX];

	/**
	 * The type of bus this is, a list of types can be found in the kernel manual
	 */
	const char type[NAME_MAX];

	/**
	 * The list of bus devices attached to the bus
	 */
	struct bus_dev_list dev_list;

	/**
	 * The current number of bus devices attached to the bus
	 */
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
 * @param[in] type The type of bus as a string, this is outlined more in the kernel manual
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
