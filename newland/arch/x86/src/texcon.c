/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/arch/texcon.h>

static uint16_t* texbuff = (uint16_t*)0xB8000;
static uint16_t cursor_x = 0;
static uint16_t cursor_y = 0;
static uint8_t texcolor = TEXCON_COLOR_ENTRY(TEXCON_COLOR_WHITE, TEXCON_COLOR_BLACK);

void texcon_clear() {
  for (uint16_t x = 0; x < 80; x++) {
    for (uint16_t y = 0; y < 25; y++) {
      const uint16_t index = y * 80 + x;
      texbuff[index] = TEXCON_ENTRY(' ', texcolor);
    }
  }
}

void arch_logc(const char c) {
  texcon_putc(c);
}

int texcon_putc(const char c) {
  if (c == '\n') {
    cursor_x = 0;
    if (cursor_y++ > 25) cursor_y = 0;
    return 0;
  }
  const uint16_t index = cursor_y * 80 + cursor_x;
  texbuff[index] = TEXCON_ENTRY(c, texcolor);
  if (cursor_x++ > 80) {
    cursor_x = 0;
    if (cursor_y++ > 25) {
      cursor_y = 0;
    }
  }
  return 1;
}

int texcon_write(const char* msg, uint16_t size) {
  int len = 0;
  for (uint16_t i = 0; i < size; i++) {
    const char c = msg[i];
    switch (c) {
      case '\n':
        cursor_x = 0;
        if (cursor_y++ > 25) {
          cursor_y = 0;
        }
        break;
      default:
        len += texcon_putc(c);
        break;
    }
  }
  return len;
}