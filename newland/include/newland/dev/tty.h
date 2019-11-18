/** \file include/newland/dev/tty.h
 * \author Tristan Ross
 * \copyright GNU Public License 3.0
 * \brief TTY Device API
 * \since v0.1.0
 */
#pragma once

#include <sys/types.h>
#include <liblist.h>
#include <limits.h>
#include <stdint.h>
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

/**
 * Gets the number of registered TTY's
 *
 * @return A number
 */
size_t tty_count();

/**
 * Gets the index of the TTY device
 *
 * @param[in] tty1 The TTY device
 * @return The index of the tty device or -1 for not found
 */
size_t tty_indexof(tty_t* tty1);

/**
 * Gets a TTY device by its index
 *
 * @param[in] i The index of the TTY device
 * @return A TTY device or NULL if not found
 */
tty_t* tty_get(size_t i);

/**
 * Gets a TTY device by its name
 *
 * @param[in] name The name of the TTY device
 * @return A TTY device or NULL if not found
 */
tty_t* tty_fromname(const char* name);

/**
 * Registers a TTY device
 *
 * @param[in] name The name of the TTY device
 * @param[in] opts The TTY device operations
 * @return Zero on success or a negative errno code
 */
int register_tty(const char* name, tty_opts_t opts);

/**
 * Unregisters a TTY device
 *
 * @param[in] name The name of the TTY device
 * @return Zero on success or a negative errno code
 */
int unregister_tty(const char* name);