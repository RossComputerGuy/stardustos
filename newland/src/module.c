/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/log.h>
#include <newland/module.h>
#include <newland/errno.h>
#include <stdint.h>

SLIST_HEAD(modlist, modinfo_t);

extern modinfo_t __modules_start[] __attribute__((weak));
extern modinfo_t __modules_end[] __attribute__((weak));
static size_t modcount = 0;
static struct modlist modules_installed = { NULL };
static struct modlist modules_loaded = { NULL };

modinfo_t* module_fromid(const char* id) {
	modinfo_t* module = NULL;
	SLIST_FOREACH(module, &modules_installed, mod_installed) {
		if (!strcmp(module->id, id)) return module;
	}
	return NULL;
}

int modules_init() {
/** Install the modules from the binary into the list **/
	size_t mc = (((uintptr_t)&__modules_end) - ((uintptr_t)&__modules_start)) / sizeof(modinfo_t);
	printk(KLOG_INFO "modules: loading %d kernel modules\n", mc);
	for (size_t i = 0; i < mc; i++) {
		modinfo_t* mod = (modinfo_t*)(((uintptr_t)&__modules_start) + (sizeof(modinfo_t) * i));
		int r = module_install(mod);
		if (r < 0) return r;
	}

/** Initialize the modules **/
	modinfo_t* module = NULL;
	SLIST_FOREACH(module, &modules_installed, mod_installed) {
		int r = module_init(module->id);
		if (r < 0) return r;
	}
	return 0;
}

int module_install(modinfo_t* module) {
	if (module_fromid(module->id) != NULL) return -NEWLAND_EEXIST;

	module->mod_installed.sle_next = NULL;
	module->mod_loaded.sle_next = NULL;

	SLIST_INSERT_HEAD(&modules_installed, module, mod_installed);
	return 0;
}

int module_init(const char* id) {
	modinfo_t* module = module_fromid(id);
	if (module == NULL) return -NEWLAND_ENOENT;
	if (module->mod_loaded.sle_next != NULL) return 0;
	int r = 0;
	if (module->deps != NULL) {
		size_t i = 0;
		while (module->deps[i++] != NULL) {
			r = module_init(module->deps[i - 1]);
			if (r < 0) return r;
		}
	}
	r = module->init();
	if (r < 0) return r;
	SLIST_INSERT_HEAD(&modules_loaded, module, mod_loaded);
	return r;
}
