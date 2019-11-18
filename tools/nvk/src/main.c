/**
 * NewLand Virtual Kernel - (C) 2019 Tristan Ross
 */
#include <argp.h>
#include <stdlib.h>
#include <time.h>

extern time_t boot_time;

/** Argument parsing **/
struct arguments {
	char* rootfs;
};

const char* arg_program_version = "NVK 1.0";
const char* argp_program_bug_address = "<spaceboyross@yandex.com>";
static char doc[] = "NewLand Virtual Kernel, a program which emulates the NewLand kernel on Linux";
static char args_doc[] = "ROOTFS";
static struct argp_option options[] = {
	{ 0 }
};

static error_t parse_opt(int key, char* arg, struct argp_state* state) {
	struct arguments* args = state->input;
	switch (key) {
		case ARGP_KEY_ARG:
			args->rootfs = arg;
			break;
		default: return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char** argv) {
	struct arguments args;
	argp_parse(&argp, argc, argv, 0, 0, &args);

	boot_time = time(NULL);
	return EXIT_SUCCESS;
}
