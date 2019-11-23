/** \file include/newland/module.h
 * \author Tristan Ross
 * \copyright GNU Public License 3.0
 * \brief Kernel Module API
 * \since v0.1.0
 * \details Static and dynamic kernel module loading and management
 */
#pragma once

#include <liblist.h>
#include <string.h>

/**
 * Module entry function
 */
typedef int (*modinit_t)();

/**
 * Module exit function
 */
typedef void (*modfini_t)();

/**
 * Kernel module info
 */
typedef struct modinfo {
	/**
	 * Module ID
	 */
	const char id[25];

	/**
	 * Author of the module
	 */
	const char author[125];

	/**
	 * License of the module
	 */
	const char license[25];

	/**
	 * Module version string
	 */
	const char modver[12];

	/**
	 * Kernel module dependencies
	 */
	const char** deps;

	/**
	 * Kernel version string
	 */
	const char krnlver[12];

	/**
	 * Module entry function
	 */
	modinit_t init;

	/**
	 * Module exit function
	 */
	modfini_t fini;

	SLIST_ENTRY(struct modinfo) mod_installed;
	SLIST_ENTRY(struct modinfo) mod_loaded;
} __attribute__((packed)) modinfo_t;

#ifdef NEWLAND_MODULE
/**
 * Defines a kernel module init function
 *
 * @param[in] id The ID of the kernel module
 */
#define MODULE_INIT(id) static int init()

/**
 * Defines a kernel module exit function
 *
 * @param[in] id The ID of the kernel module
 */
#define MODULE_FINI(id) static void fini()

/**
 * Defines a kernel module
 *
 * @param[in] id The ID of the kernel module
 * @param[in] author The author of the kernel module
 * @param[in] license The license of the module
 * @param[in] modver The version of the kernel module
 */
#define MODULE(id, author, license, modver, ...) modinfo_t kmod_## id __attribute__((section("modinfo"))) = { #id, author, license, modver, NULL, NEWLAND_VERSION, init, fini }
#define MODULE_DEPENDS(id, author, license, modver, ...) modinfo_t kmod_## id __attribute__((section("modinfo"))) = { #id, author, license, modver, { __VA_ARGS__ __VA_OPT__(,) }, NEWLAND_VERSION, init, fini }
#else
#include <newland-config.h>

/**
 * Defines a kernel module init function
 *
 * @param[in] id The ID of the kernel module
 */
#define MODULE_INIT(id) static int kmod_## id ##_init()

/**
 * Defines a kernel module exit function
 *
 * @param[in] id The ID of the kernel module
 */
#define MODULE_FINI(id) static void kmod_## id ##_fini()

/**
 * Defines a kernel module
 *
 * @param[in] id The ID of the kernel module
 * @param[in] author The author of the kernel module
 * @param[in] license The license of the module
 * @param[in] modver The version of the kernel module
 */
#define MODULE(id, author, license, modver, ...) modinfo_t kmod_## id __attribute__((section("modinfo"))) = { #id, author, license, modver, NULL, NEWLAND_VERSION, kmod_## id ##_init, kmod_## id ##_fini }
#define MODULE_DEPENDS(id, author, license, modver, ...) modinfo_t kmod_## id __attribute__((section("modinfo"))) = { #id, author, license, modver, { __VA_ARGS__ __VA_OPT__(,) }, NEWLAND_VERSION, kmod_## id ##_init, kmod_## id ##_fini }
#endif

/**
 * Finds a kernel modules by its ID
 *
 * @param[in] id The module ID
 * @return A kernel module info structure or NULL if not found
 */
modinfo_t* module_fromid(const char* id);

/**
 * Initializes all the uninitialized kernel modules
 *
 * @return Zero on success or a negative errno code
 */
int modules_init();

/**
 * Installs a kernel module
 *
 * @param[in] module The kernel module to install
 * @return Zero on success or a negative errno code
 */
int module_install(modinfo_t* module);

/**
 * Initializes a kernel module
 *
 * @param[in] id The ID of the kernel module to initialize
 * @return Zero on success or a negative errno code
 */
int module_init(const char* id);
