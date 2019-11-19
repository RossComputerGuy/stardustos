/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/dev/tty.h>
#include <newland/alloc.h>
#include <newland/dev.h>
#include <newland/log.h>
#include <newland/errno.h>

SLIST_HEAD(tty_list, tty_t);
static struct tty_list ttys = { NULL };
static size_t tty_cnt = 0;

/** Finding **/
size_t tty_count() {
	return tty_cnt;
}

size_t tty_indexof(tty_t* tty1) {
	tty_t* tty2 = NULL;
	size_t index = 0;
	SLIST_FOREACH(tty2, &ttys, tty_list) {
		if (!strcmp(tty2->name, tty1->name)) return index;
		index++;
	}
	return -1;
}

tty_t* tty_get(size_t i) {
	tty_t* tty = NULL;
	size_t index = 0;
	SLIST_FOREACH(tty, &ttys, tty_list) {
		if (index == i) return tty;
		index++;
	}
	return NULL;
}

tty_t* tty_fromname(const char* name) {
	tty_t* tty = NULL;
	SLIST_FOREACH(tty, &ttys, tty_list) {
		if (!strcmp(tty->name, name)) return tty;
	}
	return NULL;
}

/** File Node Operations **/
static fs_node_opts_t tty_fsopts = {};

/** Registration **/
int register_tty(const char* name, tty_opts_t opts) {
	if (tty_fromname(name) != NULL || device_fromname(name) != NULL) return -NEWLAND_EEXIST;
	tty_t* tty = kmalloc(sizeof(tty_t));
	if (tty == NULL) return -NEWLAND_ENOMEM;
	strcpy((char*)tty->name, name);
	tty->opts = opts;
	int r = register_device(MKDEV(DEVMAJ_TTY, tty_cnt), name, tty_fsopts, 0);
	if (r < 0) {
		kfree(tty);
		return r;
	}
	printk(KLOG_INFO "tty: registered %s\n", name);
	SLIST_INSERT_HEAD(&ttys, tty, tty_list);
	tty_cnt++;
	return 0;
}

int unregister_tty(const char* name) {
	tty_t* tty = tty_fromname(name);
	if (tty == NULL || device_fromname(name) == NULL) return -NEWLAND_ENODEV;
	size_t idx = tty_indexof(tty);
	int r = unregister_device(MKDEV(DEVMAJ_TTY, idx));
	if (r < 0) return r;
	printk(KLOG_INFO "tty: unregistered %s\n", name);
	SLIST_REMOVE(&ttys, tty, tty_t, tty_list);
	tty_cnt--;
	kfree(tty);
	return 0;
}
