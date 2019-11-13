/**
 * StardustOS libc - (C) 2019 Tristan Ross
 */
#include <libgen.h>
#include <string.h>

char* basename(char* path) {
	if (path == NULL || *path == '\0') return ".";
	char* p = path + strlen(path) - 1;
	while (*p == '/') {
		if (p == path) return path;
		*p-- = '\0';
	}
	while (p >= path && *p != '/') p--;
	return p + 1;
}
