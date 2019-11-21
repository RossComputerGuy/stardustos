/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#pragma once

#define NL_PROP_ISNVK 1

int nl_getbuildprop(int prop, void* output, int* size);