/**
 * NewLand Virtual Kernel - (C) 2019 Tristan Ross
 */
#include <newland/fs/devfs.h>
#include <newland/fs/initrd.h>
#include <newland/fs/procfs.h>
#include <newland/module.h>
#include <nvk/fs/rootfs.h>
#include <nvk/proc.h>
#include <argp.h>
#include <stdlib.h>
#include <time.h>

extern time_t boot_time;

/** Argument parsing **/
struct arguments {
	char* rootfs;
	char* initrd;
};

const char* arg_program_version = "NVK 1.0";
const char* argp_program_bug_address = "<spaceboyross@yandex.com>";
static char doc[] = "NewLand Virtual Kernel, a program which emulates the NewLand kernel on Linux";
static char args_doc[] = "ROOTFS";
static struct argp_option options[] = {
	{ "initrd", 'r', "FILE", 0, "Uses FILE for the initrd" },
	{ 0 }
};

static error_t parse_opt(int key, char* arg, struct argp_state* state) {
	struct arguments* args = state->input;
	switch (key) {
		case 'r':
			args->initrd = arg;
			break;
		case ARGP_KEY_ARG:
			args->rootfs = arg;
			break;
		default: return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char** argv) {
	struct arguments args = { NULL, NULL };
	argp_parse(&argp, argc, argv, 0, 0, &args);

	if (args.initrd == NULL && args.rootfs == NULL) {
		fprintf(stderr, "Requires the initrd or rootfs argument\n");
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

	r = mountpoint_create_fromnode(&fs, NULL, "/", 0, args.rootfs);
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
