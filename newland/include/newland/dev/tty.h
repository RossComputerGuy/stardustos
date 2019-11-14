/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#pragma once

#include <newland/types.h>
#include <liblist.h>
#include <limits.h>
#include <string.h>

struct tty;

typedef struct {
	size_t (*read)(struct tty* tty, void* buff, size_t size);
	size_t (*write)(struct tty* tty, const void* buff, size_t size);
	int (*cursor_set)(struct tty* tty, int enabled);
	int (*cursor_move)(struct tty* tty, uint32_t x, uint32_t y);
	int (*cursor_get)(struct tty* tty, uint32_t* x, uint32_t* y);
	int (*win_get)(struct tty* tty, uint32_t* x, uint32_t* y);
	int (*win_set)(struct tty* tty, uint32_t x, uint32_t y);
} tty_opts_t;

typedef struct tty {
	SLIST_ENTRY(struct tty) tty_list;
	const char name[NAME_MAX];
	tty_opts_t opts;
} tty_t;

size_t tty_count();
size_t tty_indexof(tty_t* tty1);
tty_t* tty_get(size_t i);
tty_t* tty_fromname(const char* name);

int register_tty(const char* name, tty_opts_t opts);
int unregister_tty(const char* name);
