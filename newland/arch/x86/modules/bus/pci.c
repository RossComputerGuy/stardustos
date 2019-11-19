/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/arch/io.h>
#include <newland/arch/pci.h>
#include <newland/alloc.h>
#include <newland/bus.h>
#include <newland/log.h>
#include <newland/module.h>
#include <newland/errno.h>
#include <string.h>

/** Read & Write **/
uint32_t pci_read32(pci_dev_t* dev, uint8_t reg) {
	outl(PCI_ADDR, (uint32_t)((dev->bus << 16) | (dev->slot << 11) | (dev->func << 8) | ((uint32_t)0x80000000)));
	return inl(PCI_VAL);
}

uint16_t pci_read16(pci_dev_t* dev, uint8_t reg) {
	outl(PCI_ADDR, (uint32_t)((dev->bus << 16) | (dev->slot << 11) | (dev->func << 8) | ((uint32_t)0x80000000)));
	return inw(PCI_VAL + (reg & 3));
}

uint8_t pci_read8(pci_dev_t* dev, uint8_t reg) {
	outl(PCI_ADDR, (uint32_t)((dev->bus << 16) | (dev->slot << 11) | (dev->func << 8) | ((uint32_t)0x80000000)));
	return inb(PCI_VAL + (reg & 3));
}

void pci_write32(pci_dev_t* dev, uint8_t reg, uint32_t value) {
	outl(PCI_ADDR, (uint32_t)((dev->bus << 16) | (dev->slot << 11) | (dev->func << 8) | ((uint32_t)0x80000000)));
	outl(PCI_VAL, value);
}

void pci_write16(pci_dev_t* dev, uint8_t reg, uint16_t value) {
	outl(PCI_ADDR, (uint32_t)((dev->bus << 16) | (dev->slot << 11) | (dev->func << 8) | ((uint32_t)0x80000000)));
	outw(PCI_VAL + (reg & 3), value);
}

void pci_write8(pci_dev_t* dev, uint8_t reg, uint8_t value) {
	outl(PCI_ADDR, (uint32_t)((dev->bus << 16) | (dev->slot << 11) | (dev->func << 8) | ((uint32_t)0x80000000)));
	outb(PCI_VAL + (reg & 3), value);
}

/** Scanning **/
static pci_dev_t isa;
static int hasisa = 0;

static int check_bus(uint8_t bus);

static int genres(pci_dev_t* addr, bus_dev_t** devptr, uint8_t barindex) {
	bus_dev_t* dev = *devptr;
	barindex *= 4;
	barindex += 16;
	uint32_t barval = pci_read32(addr, barindex);
	pci_write32(addr, barindex, 0xFFFFFFFF);
	uint32_t barsize = pci_read32(addr, barindex);
	if (barsize == 0 || barval == 0) return 0;
	barsize = (~barsize) + 1;
	if (barsize == 0) return 0;
	bus_dev_res_t* res = kmalloc(sizeof(bus_dev_res_t));
	if (res == NULL) return -NEWLAND_ENOMEM;
	res->size = barsize;
	if (barval & 0x4) {
		res->value = barval & 0xFFFFFFF0;
		res->type = BUSDEVRES_MEM;
	} else if (barval & 0x1) {
		res->value = (barval & ~0x3) & 0xFFFF;
		res->type = BUSDEVRES_IO;
	} else {
		res->value = barval & 0xFFFFFFF0;
		res->type = BUSDEVRES_MEM;
	}
	if (res->value == 0 || res->size == 0) {
		kfree(res);
		return 0;
	}
	SLIST_INSERT_HEAD(&dev->res_list, res, res_list);
	dev->res_count++;
	return 0;
}

static int found_dev(pci_dev_t* addr) {
	uint16_t did = pcidev_getdevice(addr);
	uint16_t vid = pcidev_getvendor(addr);
	if (vid == 0xFFFF) return 0;
	char name[9];
	memset(name, 0, 9);
	strcpy(name, "000.00.0");
	itoa(name, 10, addr->bus);
	if (name[1] == 0) name[1] = '0';
	if (name[2] == 0) name[2] = '0';
	name[3] = '.';
	itoa(name + 4, 10, addr->slot);
	if (name[5] == 0) name[5] = '0';
	name[6] = '.';
	itoa(name + 7, 10, addr->func);
	bus_t* bus = bus_fromname("pci");
	bus_adddev(bus, name);
	bus_dev_t* bdev = bus_getdevbyname(bus, name);
	uint8_t intr = pci_getint(addr);
	if (intr != 0) {
		bdev->flags |= BUSDEV_INT;
		bdev->interrupt = intr;
	}
	bdev->addr = (uint32_t)((addr->bus << 16) | (addr->slot << 11) | (addr->func << 8));
	if ((pcidev_gethdrtype(addr) & 0x80) == 0) {
		for (uint8_t i = 0; i < 6; i++) {
			int r = genres(addr, &bdev, i);
			if (r < 0) return r;
		}
	}
	if (vid == 0x8086 && (did == 0x7000 || did == 0x7110)) {
		isa = (pci_dev_t){ addr->bus, addr->slot, addr->func };
		hasisa = 1;
	}
	printk(KLOG_INFO "pci: found device %s\n", name);
	return 0;
}

static int check_func(uint8_t bus, uint8_t dev, uint8_t func) {
	pci_dev_t addr = { bus, dev, func };
	uint16_t vid = pcidev_getvendor(&addr);
	if (vid == 0xFFFF) return 0;
	int r = found_dev(&addr);
	if (r < 0) return r;
	uint8_t class = (pci_read32(&addr, 8) >> 24) & 0xFF;
	uint8_t subclass = (pci_read32(&addr, 8) >> 16) & 0xFF;
	if (class == 0x06 && subclass == 0x04) {
		if ((r = check_bus(pcidev_get2ndbus(&addr))) < 0) return r;
	}
	return 0;
}

static int check_dev(uint8_t bus, uint8_t dev) {
	pci_dev_t addr = { bus, dev, 0 };
	uint16_t vid = pcidev_getvendor(&addr);
	if (vid == 0xFFFF) return 0;
	for (uint8_t func = 0; func < 8; func++) {
		addr.func = func;
		vid = pcidev_getvendor(&addr);
		if (vid != 0xFFFF) {
			int r = check_func(bus, dev, func);
			if (r < 0) return r;
		}
	}
	return 0;
}

static int check_bus(uint8_t bus) {
	for (uint8_t dev = 0; dev < 32; dev++) {
		int r = check_dev(bus, dev);
		if (r < 0) return r;
	}
	return 0;
}

static int scan_buses() {
	pci_dev_t addr = { 0, 0, 0 };
	uint8_t header_type = pcidev_gethdrtype(&addr);
	if ((header_type & 0x80) == 0) {
		int r = check_bus(0);
		if (r < 0) return r;
	} else {
		for (uint8_t func = 0; func < 8; func++) {
			addr.func = func;
			uint16_t vid = pcidev_getvendor(&addr);
			if (vid != 0xFFFF) {
				int r = check_bus(func);
				if (r < 0) return r;
			}
		}
	}
	return 0;
}

/** Interrupt stuff **/
static uint32_t remaps[4] = { 0 };

uint8_t pci_getint(pci_dev_t* addr) {
	if (hasisa) {
		uint8_t irqpin = pcidev_getintpin(addr);
		if (irqpin == 0) return pcidev_getintline(addr);
		int pirq = (irqpin + addr->slot - 2) % 4;
		uint8_t intline = pcidev_getintline(addr);
		if (remaps[pirq] >= 0x80) {
			if (intline == 0xFF) {
				intline = 10;
				pci_write32(addr, 60, (intline | 0xFF) << 16);
				remaps[pirq] = intline;
			}
			return intline;
		}
		pci_write32(addr, 60, (remaps[pirq] | 0xFF) << 16);
		return remaps[pirq];
	} else return pcidev_getintline(addr);
}

/** Module Stuff **/
MODULE_INIT(bus_pci) {
	int r = register_bus(NULL, "pci");
	if (r < 0) return r;
	r = scan_buses();
	bus_t* bus = bus_fromname("pci");
	printk(KLOG_INFO "pci: found %d devices\n", bus->dev_count);
	bus_dev_t* dev = NULL;
	return r;
}

MODULE_FINI(bus_pci) {
	unregister_bus("pci");
}

MODULE(bus_pci, "Tristan Ross", "GPL", "0.1.0");
