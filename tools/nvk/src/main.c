/**
 * NewLand Virtual Kernel - (C) 2019 Tristan Ross
 */
#include <newland/fs/devfs.h>
#include <newland/fs/initrd.h>
#include <newland/fs/procfs.h>
#include <newland/module.h>
#include <nvk/fs/rootfs.h>
#include <nvk/proc.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

extern time_t boot_time;

int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "Requires the rootfs argument\n");
		return EXIT_FAILURE;
	}

	boot_time = time(NULL);

	int r = procfs_init();
	if (r < 0) {
		fprintf(stderr, "Failed to initialize procfs: %d\n", r);
		return EXIT_FAILURE;
	}

	r = initrd_init();
	if (r < 0) {
		fprintf(stderr, "Failed to initialize initrd: %d\n", r);
		return EXIT_FAILURE;
	}

	r = modules_init();
	if (r < 0) {
		fprintf(stderr, "Failed to load kernel modules: %d\n", r);
		return EXIT_FAILURE;
	}

	r = rootfs_init();
	if (r < 0) {
		fprintf(stderr, "Failed to initialize rootfs: %d\n", r);
		return EXIT_FAILURE;
	}

	r = devfs_init();
	if (r < 0) {
		fprintf(stderr, "Failed to load kernel modules: %d\n", r);
		return EXIT_FAILURE;
	}

	fs_t* fs = fs_fromname("rootfs");
	if (fs == NULL) {
		fprintf(stderr, "Failed to find fs: rootfs\n");
		return EXIT_FAILURE;
	}

	r = mountpoint_create_fromnode(&fs, NULL, "/", 0, argv[1]);
	if (r < 0) {
		fprintf(stderr, "Failed to find mount rootfs: %d\n", r);
		return EXIT_FAILURE;
	}

	r = proc_exec("/init", NULL);
	if (r < 0) {
		fprintf(stderr, "Failed to execute init process: %d\n", r);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
