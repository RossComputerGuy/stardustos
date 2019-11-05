/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#pragma once

#include <newland/types.h>

#define PCI_ADDR 0xCF8
#define PCI_VAL 0xCFC

typedef struct {
  uint8_t bus, slot, func;
} pci_dev_t;

#define pcidev_getdevice(dev) (pci_read32(dev, 0) >> 16)
#define pcidev_getvendor(dev) (pci_read32(dev, 0) & 0xFFFF)
#define pcidev_gethdrtype(dev) ((pci_read32(dev, 12) >> 16) & 0xFF)

uint32_t pci_read32(pci_dev_t* dev, uint8_t reg);
uint16_t pci_read16(pci_dev_t* dev, uint8_t reg);
uint8_t pci_read8(pci_dev_t* dev, uint8_t reg);

void pci_write32(pci_dev_t* dev, uint8_t reg, uint32_t value);
void pci_write16(pci_dev_t* dev, uint8_t reg, uint16_t value);
void pci_write8(pci_dev_t* dev, uint8_t reg, uint8_t value);
