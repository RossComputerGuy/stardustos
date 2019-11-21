/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/errno.h>
#include <sys/newland.h>

int nl_getbuildprop(int prop, void* output, int* size) {
	switch (prop) {
#ifdef __nvk__
		case NL_PROP_NVK_VERSION:
			break;
#endif
		default: return -NEWLAND_EINVAL;
	}
	return 0;
}