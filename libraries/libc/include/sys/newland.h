/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#pragma once

#define NL_PROP_NVK_VERSION 1

int nl_getbuildprop(int prop, void* output, int* size);