/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#pragma once

#define NEWLAND_SYS_nl_getbuildprop 0

#ifndef HAS_LIBC
#define SYS_open NEWLAND_SYS_nl_getbuildprop
#endif
