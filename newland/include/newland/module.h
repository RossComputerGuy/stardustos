/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#pragma once

#include <string.h>

typedef int (*modinit_t)();
typedef void (*modfini_t)();

typedef struct {
	const char id[25];
	const char author[125];
	const char license[25];
	const char modver[12];
	const char krnlver[12];
	modinit_t init;
	modfini_t fini;
} __attribute__((packed)) modinfo_t;

#ifdef NEWLAND_MODULE
#define MODULE_INIT(id) static int init()
#define MODULE_FINI(id) static void fini()
#define MODULE(id, author, license, modver) modinfo_t kmod_## id __attribute__((section("modinfo"))) = { #id, author, license, modver, NEWLAND_VERSION, init, fini }
#else
#include <newland-config.h>
#define MODULE_INIT(id) static int kmod_## id ##_init()
#define MODULE_FINI(id) static void kmod_## id ##_fini()
#define MODULE(id, author, license, modver) modinfo_t kmod_## id __attribute__((section("modinfo"))) = { #id, author, license, modver, NEWLAND_VERSION, kmod_## id ##_init, kmod_## id ##_fini }
#endif

size_t module_count();
modinfo_t* module_fromid(const char* id);
int modules_init();
