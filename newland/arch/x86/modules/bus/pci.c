/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/arch/io.h>
#include <newland/bus.h>
#include <newland/log.h>
#include <newland/module.h>
#include <newland/types.h>
#include <string.h>

#define PCI_ADDR_PORT 0xCF8
#define PCI_VAL_PORT 0xCFC

#define PCI_VENDOR_ID 0x00
#define PCI_DEVICE_ID 0x02
#define PCI_COMMAND 0x04
#define PCI_STATUS 0x06
#define PCI_REVISION_ID 0x08

#define PCI_PROG_IF 0x09
#define PCI_SUBCLASS 0x0a
#define PCI_CLASS 0x0b
#define PCI_CACHE_LINE_SIZE 0x0c
#define PCI_LATENCY_TIMER 0x0d
#define PCI_HEADER_TYPE 0x0e
#define PCI_BIST 0x0f
#define PCI_BAR0 0x10
#define PCI_BAR1 0x14
#define PCI_BAR2 0x18
#define PCI_BAR3 0x1C
#define PCI_BAR4 0x20
#define PCI_BAR5 0x24

#define PCI_INTERRUPT_LINE 0x3C

#define PCI_SECONDARY_BUS 0x19

#define PCI_HEADER_TYPE_DEVICE 0
#define PCI_HEADER_TYPE_BRIDGE 1
#define PCI_HEADER_TYPE_CARDBUS 2

#define PCI_TYPE_BRIDGE 0x0604

#define pci_extract_bus(dev) ((dev) >> 16)
#define pci_extract_slot(dev) ((dev) >> 8)
#define pci_extract_func(dev) ((dev))
#define pci_getaddr(dev, field) 0x80000000 | (pci_extract_bus((dev)) << 16) | (pci_extract_slot((dev)) << 11) | (pci_extract_func((dev)) << 8) | ((field) & 0xFC)
#define pci_findtype(dev) (read_field(dev, PCI_CLASS, 1) << 8) | read_field(dev, PCI_SUBCLASS, 1)
#define pci_boxdev(bus, slot, func) (((bus) << 16) | ((slot) << 8) | (func))

/** Read & Write **/
static void write_field(uint32_t dev, int field, uint32_t value) {
  outl(PCI_ADDR_PORT, pci_getaddr(dev, field));
  outl(PCI_VAL_PORT, value);
}

static uint32_t read_field(uint32_t dev, int field, int size) {
  outl(PCI_ADDR_PORT, pci_getaddr(dev, field));
  switch (size) {
    case 1: return inb(PCI_VAL_PORT + (field & 3));
    case 2: return inw(PCI_VAL_PORT + (field & 2));
    case 4: return inl(PCI_VAL_PORT);
    default: return 0xFFFF;
  }
}

/** Scanning **/
static void scan_bus(int type, int bus);

static void scan_func(int type, int bus, int slot, int func) {
  uint32_t dev = pci_boxdev(bus, slot, func);
  if (type == -1 || type == pci_findtype(dev)) {
    uint16_t vid = read_field(dev, PCI_VENDOR_ID, 2);
    uint16_t did = read_field(dev, PCI_DEVICE_ID, 2);
    char name[11];
    memset(name, 0, 11);
    strcpy(name, "000.000.000");
    itoa(name, 10, bus);
    if (name[1] == 0) name[1] = '0';
    if (name[2] == 0) name[2] = '0';
    name[3] = '.';
    itoa(name + 4, 10, slot);
    if (name[5] == 0) name[5] = '0';
    if (name[6] == 0) name[6] = '0';
    name[7] = '.';
    itoa(name + 8, 10, func);
    if (name[9] == 0) name[9] = '0';
    if (name[10] == 0) name[10] = '0';
    bus_adddev(bus_fromname("pci"), name);
  }
  if (pci_findtype(dev) == PCI_TYPE_BRIDGE) {
    scan_bus(type, read_field(dev, PCI_SECONDARY_BUS, 1));
  }
}

static void scan_slot(int type, int bus, int slot) {
  uint32_t dev = pci_boxdev(bus, slot, 0);
  if (read_field(dev, PCI_VENDOR_ID, 2) == 0xFFFF) return;
  scan_func(type, bus, slot, 0);
  if ((read_field(dev, PCI_HEADER_TYPE, 1) & 0x80) != 0) {
    for (int func = 1; func < 8; func++) {
      dev = pci_boxdev(bus, slot, func);
      if (read_field(dev, PCI_VENDOR_ID, 2) != 0xFFFF) scan_func(type, bus, slot, func);
    }
  }
}

static void scan_bus(int type, int bus) {
  for (int slot = 0; slot < 32; slot++) {
    scan_slot(type, bus, slot);
  }
}

static void scan(int type) {
  if ((read_field(0, PCI_HEADER_TYPE, 1) & 0x80) == 0) scan_bus(type, 0);
  else {
    for (int func = 0; func < 8; func++) {
      uint32_t dev = pci_boxdev(0, 0, func);
      if (read_field(dev, PCI_VENDOR_ID, 2) != 0xFFFF) scan_bus(type, func);
      else break;
    }
  }
}

/** Module Stuff **/
MODULE_INIT(bus_pci) {
  int r = register_bus(NULL, "pci");
  if (r < 0) return r;
  scan(-1);
  return 0;
}

MODULE_FINI(bus_pci) {
  unregister_bus("pci");
}

MODULE(bus_pci, "Tristan Ross", "GPL", "0.1.0");
