/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#pragma once

#include <newland/types.h>

typedef enum {
  TEXCON_COLOR_BLACK = 0,
	TEXCON_COLOR_BLUE = 1,
	TEXCON_COLOR_GREEN = 2,
	TEXCON_COLOR_CYAN = 3,
	TEXCON_COLOR_RED = 4,
	TEXCON_COLOR_MAGENTA = 5,
	TEXCON_COLOR_BROWN = 6,
	TEXCON_COLOR_LIGHT_GREY = 7,
	TEXCON_COLOR_DARK_GREY = 8,
	TEXCON_COLOR_LIGHT_BLUE = 9,
	TEXCON_COLOR_LIGHT_GREEN = 10,
	TEXCON_COLOR_LIGHT_CYAN = 11,
	TEXCON_COLOR_LIGHT_RED = 12,
	TEXCON_COLOR_LIGHT_MAGENTA = 13,
	TEXCON_COLOR_LIGHT_BROWN = 14,
	TEXCON_COLOR_WHITE = 15
} texcon_color_e;

#define TEXCON_COLOR_ENTRY(fg, bg) (fg | bg << 4)
#define TEXCON_ENTRY(uc, color) (uc | color << 8)

void texcon_clear();
int texcon_putc(const char c);
int texcon_write(const char* msg, uint16_t size);

#define texcon_puts(msg) texcon_write(msg, strlen(msg))