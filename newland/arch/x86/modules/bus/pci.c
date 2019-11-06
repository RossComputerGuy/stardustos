/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/arch/io.h>
#include <newland/arch/pci.h>
#include <newland/bus.h>
#include <newland/log.h>
#include <newland/module.h>
#include <newland/types.h>
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

static void check_bus(uint8_t bus);

static void found_dev(pci_dev_t* addr) {
  uint16_t did = pcidev_getdevice(addr);
  uint16_t vid = pcidev_getvendor(addr);
  if (vid == 0xFFFF) return;
  char name[12];
  memset(name, 0, 12);
  strcpy(name, "000.000.000");
  itoa(name, 10, addr->bus);
  if (name[1] == 0) name[1] = '0';
  if (name[2] == 0) name[2] = '0';
  name[3] = '.';
  itoa(name + 4, 10, addr->slot);
  if (name[5] == 0) name[5] = '0';
  if (name[6] == 0) name[6] = '0';
  name[7] = '.';
  itoa(name + 8, 10, addr->func);
  if (name[9] == 0) name[9] = '0';
  if (name[10] == 0) name[10] = '0';
  name[11] = 0;
  bus_t* bus = bus_fromname("pci");
  bus_adddev(bus, name);
  bus_dev_t* bdev = bus_getdevbyname(bus, name);
  uint8_t intr = pci_getint(addr);
  if (intr != 0) {
    bdev->flags |= BUSDEV_INT;
    bdev->interrupt = intr;
  }
  if (vid == 0x8086 && (did == 0x7000 || did == 0x7110)) {
    isa = (pci_dev_t){ addr->bus, addr->slot, addr->func };
    hasisa = 1;
  }
}

static void check_func(uint8_t bus, uint8_t dev, uint8_t func) {
  pci_dev_t addr = { bus, dev, func };
  uint16_t vid = pcidev_getvendor(&addr);
  if (vid == 0xFFFF) return;
  found_dev(&addr);
  uint8_t class = (pci_read32(&addr, 8) >> 24) & 0xFF;
  uint8_t subclass = (pci_read32(&addr, 8) >> 16) & 0xFF;
  if (class == 0x06 && subclass == 0x04) check_bus(pcidev_get2ndbus(&addr));
}

static void check_dev(uint8_t bus, uint8_t dev) {
  pci_dev_t addr = { bus, dev, 0 };
  uint16_t vid = pcidev_getvendor(&addr);
  if (vid == 0xFFFF) return;
  check_func(bus, dev, 0);
  uint8_t header_type = pcidev_gethdrtype(&addr);
  if ((header_type & 0x80) != 0) {
    for (uint8_t func = 1; func < 8; func++) {
      addr.func = func;
      vid = pcidev_getvendor(&addr);
      if (vid != 0xFFFF) check_func(bus, dev, func);
    }
  }
}

static void check_bus(uint8_t bus) {
  for (uint8_t dev = 0; dev < 32; dev++) check_dev(bus, dev);
}

static void scan_buses() {
  pci_dev_t addr = { 0, 0, 0 };
  uint8_t header_type = pcidev_gethdrtype(&addr);
  if ((header_type & 0x80) == 0) check_bus(0);
  else {
    for (uint8_t func = 0; func < 8; func++) {
      addr.func = func;
      uint16_t vid = pcidev_getvendor(&addr);
      if (vid != 0xFFFF) break;
      check_bus(func);
    }
  }
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
  scan_buses();
  bus_t* bus = bus_fromname("pci");
  printk(KLOG_INFO "pci: found %d devices\n", bus->dev_count);
  bus_dev_t* dev = NULL;
  return 0;
}

MODULE_FINI(bus_pci) {
  unregister_bus("pci");
}

MODULE(bus_pci, "Tristan Ross", "GPL", "0.1.0");
