/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#pragma once

#define NEWLAND_SYS_nl_getbuildprop 0
#define NEWLAND_SYS_nl_open 1
#define NEWLAND_SYS_nl_close 2
#define NEWLAND_SYS_nl_write 3
#define NEWLAND_SYS_nl_read 4

#ifndef HAS_LIBC
#define SYS_nl_getbuildprop NEWLAND_SYS_nl_getbuildprop
#define SYS_nl_open NEWLAND_SYS_nl_open
#define SYS_nl_close NEWLAND_SYS_nl_close
#define SYS_nl_write NEWLAND_SYS_nl_write
#define SYS_nl_read NEWLAND_SYS_nl_read
#endif
