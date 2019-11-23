/**
 * StardustOS libnewland - (C) 2019 Tristan Ross
 */
#pragma once

#include <limits.h>

#define NL_DISP_WRITINF 0
#define NL_DISP_READINF 1

typedef struct {
	/**
	 * Writable resolution value of the width
	 */
	uint32_t res_width;

	/**
	 * Writable resolution value of the height
	 */
	uint32_t res_height;

	/**
	 * Writable depth value
	 */
	uint8_t res_depth;

	/**
	 * Writable buffer of the display, needs to be memory mapped to be usable
	 */
	void* buffer;

	/**
	 * Read-only field of the display name
	 */
	const char name[NAME_MAX];

	/**
	 * Writable, bit that enables or disables the display
	 */
	int active:1;
} nl_display_t;

#define NL_GFXDEV_READINF 1

typedef struct {
	int dispcount;
	const char name[NAME_MAX];
} nl_gfxdev_t;
