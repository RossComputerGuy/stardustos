/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/arch/proc.h>
#include <newland/errno.h>
#include <sys/newland.h>

int nl_getbuildprop(int prop, void* output, int* size) {
	switch (prop) {
#ifdef __nvk__
		case NL_PROP_ISNVK: return 1;
#else
		case NL_PROP_ISNVK: return 0;
#endif
		default: return -NEWLAND_EINVAL;
	}
	return 0;
}

pid_t nl_getpid() {
	proc_t* proc = proccess_curr();
	if (proc == NULL) return 0;
	return proc->id;
}
