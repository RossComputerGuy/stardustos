/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/arch/io.h>
#include <newland/dev/tty.h>
#include <newland/log.h>
#include <newland/module.h>
#include <newland/errno.h>
#include <string.h>

static uint16_t comport[4] = { 0x3F8, 0x2F8, 0x3E8, 0x2E8 };

/** Reading **/
static int isreceived(uint8_t comindex) {
	return inb(comport[comindex] + 5) & 1;
}

static char read_char(uint8_t comindex) {
	while (isreceived(comindex) == 0);
	return inb(comport[comindex]);
}

/*** Writing **/
static int istransmit(uint8_t comindex) {
	return inb(comport[comindex] + 5) & 0x20;
}

static void write_char(uint8_t comindex, char c) {
	while (istransmit(comindex) == 0);
	outb(comport[comindex], c);
}

/** TTY Stuff **/
static size_t tty_read(tty_t* tty, void* buff, size_t size) {
	char* cbuff = (char*)buff;
	for (size_t i = 0; i < size; i++) cbuff[i] = read_char(tty->name[3] - '0');
	return size;
}

static size_t tty_write(tty_t* tty, const void* buff, size_t size) {
	const char* cbuff = (const char*)buff;
	for (size_t i = 0; i < size; i++) write_char(tty->name[3] - '0', cbuff[i]);
	return size;
}

static tty_opts_t tty_opts = { .read = tty_read, .write = tty_write };

static int tty_creat(uint8_t comindex) {
	char name[5];
	memset(name, 0, 4);
	strcpy(name, "ser");
	itoa((char*)(name + 3), 10, comindex);
	name[4] = 0;
	outb(comport[comindex] + 1, 0x00);
	outb(comport[comindex] + 3, 0x80);
	outb(comport[comindex] + 0, 0x03);
	outb(comport[comindex] + 1, 0x00);
	outb(comport[comindex] + 3, 0x03);
	outb(comport[comindex] + 2, 0xC7);
	outb(comport[comindex] + 4, 0x0B);
	return register_tty(name, tty_opts);
}

/** Module Stuff **/
MODULE_INIT(tty_serial) {
	for (uint8_t i = 0; i < 4; i++) {
		int r = tty_creat(i);
		if (r < 0) return r;
	}
	return 0;
}

MODULE_FINI(tty_serial) {
	for (uint8_t i = 0; i < 4; i++) {
		char name[4];
		memset(name, 0, 4);
		strcpy(name, "ser");
		itoa(name + 3, 10, i);
		unregister_tty(name);
	}
}

MODULE(tty_serial, "Tristan Ross", "GPL", "0.1.0");
