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
void pci_writefield(uint32_t dev, int field, uint32_t value) {
  outl(PCI_ADDR_PORT, pci_getaddr(dev, field));
  outl(PCI_VAL_PORT, value);
}

uint32_t pci_readfield(uint32_t dev, int field, int size) {
  outl(PCI_ADDR_PORT, pci_getaddr(dev, field));
  switch (size) {
    case 1: return inb(PCI_VAL_PORT + (field & 3));
    case 2: return inw(PCI_VAL_PORT + (field & 2));
    case 4: return inl(PCI_VAL_PORT);
    default: return 0xFFFF;
  }
}

/** Scanning **/
static uint32_t isa = 0;

static void scan_bus(int type, int bus);

static void scan_func(int type, int bus, int slot, int func) {
  uint32_t dev = pci_boxdev(bus, slot, func);
  if (type == -1 || type == pci_findtype(dev)) {
    uint16_t vid = pci_readfield(dev, PCI_VENDOR_ID, 2);
    uint16_t did = pci_readfield(dev, PCI_DEVICE_ID, 2);
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
    bus_t* bus = bus_fromname("pci");
    bus_adddev(bus, name);
    if (vid == 0x8086 && (did == 0x7000 || did == 0x7110)) isa = dev;
    bus_dev_t* bdev = bus_getdevbyname(bus, name);
    int interrupt = pci_getint(dev);
    if (interrupt != 0) {
      bdev->flags |= BUSDEV_INT;
      bdev->interrupt = interrupt;
    }
    printk(KLOG_INFO "pci: added device %s\n", name);
  }
  /*if (pci_findtype(dev) == PCI_TYPE_BRIDGE) {
    scan_bus(type, pci_readfield(dev, PCI_SECONDARY_BUS, 1));
  }*/
}

static void scan_slot(int type, int bus, int slot) {
  uint32_t dev = pci_boxdev(bus, slot, 0);
  if (pci_readfield(dev, PCI_VENDOR_ID, 2) == 0xFFFF) return;
  scan_func(type, bus, slot, 0);
  if ((pci_readfield(dev, PCI_HEADER_TYPE, 1) & 0x80) != 0) {
    for (int func = 1; func < 8; func++) {
      dev = pci_boxdev(bus, slot, func);
      if (pci_readfield(dev, PCI_VENDOR_ID, 2) != 0xFFFF) scan_func(type, bus, slot, func);
    }
  }
}

static void scan_bus(int type, int bus) {
  for (int slot = 0; slot < 32; slot++) {
    scan_slot(type, bus, slot);
  }
}

static void scan(int type) {
  if ((pci_readfield(0, PCI_HEADER_TYPE, 1) & 0x80) == 0) scan_bus(type, 0);
  else {
    for (int func = 0; func < 8; func++) {
      uint32_t dev = pci_boxdev(0, 0, func);
      if (pci_readfield(dev, PCI_VENDOR_ID, 2) != 0xFFFF) scan_bus(type, func);
      else break;
    }
  }
}

/** Interrupt stuff **/
static uint32_t remaps[4] = { 0 };

int pci_getint(uint32_t dev) {
  if (isa) {
    uint32_t irqpin = pci_readfield(dev, 0x3D, 1);
    if (irqpin == 0) return pci_readfield(dev, PCI_INTERRUPT_LINE, 1);
    int pirq = (irqpin + pci_extract_slot(dev) - 2) % 4;
    int intline = pci_readfield(dev, PCI_INTERRUPT_LINE, 1);
    if (remaps[pirq] >= 0x80) {
      if (intline == 0xFF) {
        intline = 10;
        pci_writefield(dev, PCI_INTERRUPT_LINE, intline);
      }
      remaps[pirq] = intline;
      uint32_t out = 0;
      memcpy(&out, &remaps, 4);
      pci_writefield(isa, 0x60, out);
      return intline;
    }
    pci_writefield(dev, PCI_INTERRUPT_LINE, remaps[pirq]);
    return remaps[pirq];
  } else return pci_readfield(dev, PCI_INTERRUPT_LINE, 1);
}

/** Module Stuff **/
MODULE_INIT(bus_pci) {
  int r = register_bus(NULL, "pci");
  if (r < 0) return r;
  for (uint8_t b = 0; b < 255; b++) {
    for (uint8_t s = 0; s < 32; s++) {
      uint32_t dev = pci_boxdev(b, s, 0);
      if (pci_readfield(dev, PCI_VENDOR_ID, 2) == 0xFFFF) continue;
      scan_func(-1, b, s, 0);
      if ((pci_readfield(dev, PCI_HEADER_TYPE, 1) & 0x80) != 0) {
        for (uint8_t f = 1; f < 8; f++) scan_func(-1, b, s, f);
      }
    }
  }
  return 0;
}

MODULE_FINI(bus_pci) {
  unregister_bus("pci");
}

MODULE(bus_pci, "Tristan Ross", "GPL", "0.1.0");
