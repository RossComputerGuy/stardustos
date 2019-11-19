/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#pragma once

#define NEWLAND_SYS_open 1
#define NEWLAND_SYS_close 2

#ifndef HAS_LIBC
#define SYS_open NEWLAND_SYS_open
#define SYS_close NEWLAND_SYS_close
#endif
