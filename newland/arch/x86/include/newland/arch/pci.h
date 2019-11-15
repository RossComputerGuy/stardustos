/** \file arch/x86/include/newland/arch/pci.h
 * \author Tristan Ross
 * \copyright GNU Public License 3.0
 * \brief x86 pci functions
 * \since v0.1.0
 */
#pragma once

#include <sys/types.h>

/**
 * Address for setting which device to use
 */
#define PCI_ADDR 0xCF8

/**
 * PCI Device field
 */
#define PCI_VAL 0xCFC

/**
 * Structure for easily accessing a device
 */
typedef struct {
	uint8_t bus, slot, func;
} pci_dev_t;

/**
 * Get the device ID of a device
 *
 * @param[in] dev The device access structure pointer
 * @return The device ID
 */
#define pcidev_getdevice(dev) (pci_read32(dev, 0) >> 16)

/**
 * Get the vendor ID of a device
 *
 * @param[in] dev The device access structure pointer
 * @return The vendor ID
 */
#define pcidev_getvendor(dev) (pci_read32(dev, 0) & 0xFFFF)

/**
 * Get the header type of the device
 *
 * @param[in] dev The device access structure pointer
 * @return The header type
 */
#define pcidev_gethdrtype(dev) ((pci_read32(dev, 12) >> 16) & 0xFF)

/**
 * Get the secondary bus of a device
 *
 * @param[in] dev The device access structure pointer
 * @return The secondary bus
 */
#define pcidev_get2ndbus(dev) ((pci_read32(dev, 24) >> 24) & 0xFF)

/**
 * Get the interrupt pin of a device
 *
 * @param[in] dev The device access structure pointer
 * @return The interrupt pin
 */
#define pcidev_getintpin(dev) ((pci_read32(dev, 60) >> 24) & 0xFF)

/**
 * Get the interrupt line of a device
 *
 * @param[in] dev The device access structure pointer
 * @return The interrupt line
 */
#define pcidev_getintline(dev) ((pci_read32(dev, 60) >> 16) & 0xFF)

/**
 * Read a uint32_t from a PCI device
 *
 * @param[in] dev The device to read from
 * @param[in] reg The register to read
 * @return The value of the register
 */
uint32_t pci_read32(pci_dev_t* dev, uint8_t reg);

/**
 * Read a uint16_t from a PCI device
 *
 * @param[in] dev The device to read from
 * @param[in] reg The register to read
 * @return The value of the register
 */
uint16_t pci_read16(pci_dev_t* dev, uint8_t reg);

/**
 * Read a uint8_t from a PCI device
 *
 * @param[in] dev The device to read from
 * @param[in] reg The register to read
 * @return The value of the register
 */
uint8_t pci_read8(pci_dev_t* dev, uint8_t reg);

/**
 * Write a uint32_t to a PCI device
 *
 * @param[in] dev The device to write to
 * @param[in] reg The register to write to
 * @param[in] value The value to write
 */
void pci_write32(pci_dev_t* dev, uint8_t reg, uint32_t value);

/**
 * Write a uint16_t to a PCI device
 *
 * @param[in] dev The device to write to
 * @param[in] reg The register to write to
 * @param[in] value The value to write
 */
void pci_write16(pci_dev_t* dev, uint8_t reg, uint16_t value);

/**
 * Write a uint8_t to a PCI device
 *
 * @param[in] dev The device to write to
 * @param[in] reg The register to write to
 * @param[in] value The value to write
 */
void pci_write8(pci_dev_t* dev, uint8_t reg, uint8_t value);

/**
 * Get the interrupt number of the PCI device
 *
 * @param[in] addr The device
 * @return The IRQ number
 */
uint8_t pci_getint(pci_dev_t* addr);
