/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/arch/io.h>
#include <newland/time.h>
#include <string.h>

time_t boot_time = 0;

#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

#define from_bcd(val) (((val) / 16) * 10 + ((val)&0xf))
#define cmos_wait() while (is_cmos_update())

#define T_SECOND 0x0
#define T_MINUTE 0x2
#define T_HOUR 0x4
#define T_DAY 0x7
#define T_MONTH 0x8
#define T_YEAR 0x9

static int is_cmos_update() {
	outb(CMOS_ADDRESS, 0x0A);
	return (inb(CMOS_DATA) & 0x80);
}

static char get_realtime_reg(int reg) {
	outb(CMOS_ADDRESS, reg);
	return inb(CMOS_DATA);
}

int arch_clock(datetime_t* dt) {
	cmos_wait();
	dt->sec = from_bcd(get_realtime_reg(T_SECOND));
	dt->min = from_bcd(get_realtime_reg(T_MINUTE));
	dt->hr = from_bcd(get_realtime_reg(T_HOUR));
	dt->day = from_bcd(get_realtime_reg(T_DAY));
	dt->mon = from_bcd(get_realtime_reg(T_MONTH));
	dt->yr = from_bcd(get_realtime_reg(T_YEAR));
	return 0;
}

time_t boottime(time_t* t) {
	time_t i = time(NULL) - boot_time;
	if (t != NULL) *t = i;
	return i;
}