/**
 * StardustOS libfile - (C) 2019 Tristan Ross
 */
#pragma once

#include <stdint.h>

#define ELF_NIDENT 16

#define ELFMAG0 0x7F
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'

typedef enum {
	ELF_NONE = 0,
	ELF_REL = 1,
	ELF_EXEC = 2,
	ELF_SHAR = 3
} elf_type_t;

typedef struct {
	uint8_t ident[ELF_NIDENT];
	uint16_t type;
	uint16_t machine;
	uint32_t version;
	uint32_t entry;
	uint32_t phoff;
	uint32_t shoff;
	uint32_t flags;
	uint16_t ehsize;
	uint16_t phentsize;
	uint16_t phnum;
	uint16_t shentsize;
	uint16_t shnum;
	uint16_t shstrndx;
} elf_header_t;

typedef enum {
	ELFS_NULL = 0,
	ELFS_PROGBITS = 1,
	ELFS_SYMTAB = 2,
	ELFS_STRTAB = 3,
	ELFS_RELA = 4,
	ELFS_NOBITS = 8,
	ELFS_REL = 9
} elf_section_type_t;

typedef struct {
	uint8_t name;
	uint8_t type;
	uint8_t flags;
	uint8_t addr;
	uint8_t offset;
	uint8_t size;
	uint8_t link;
	uint8_t info;
	uint8_t addralign;
	uint8_t entsize;
} elf_section_t;

#define ELF_PROG_EXEC (1 << 0)
#define ELF_PROG_WRITE (1 << 1)
#define ELF_PROG_READ (1 << 2)

typedef struct {
	uint32_t type;
	uint32_t offset;
	uint32_t vaddr;
	uint32_t paddr;
	uint32_t filesz;
	uint32_t memsz;
	uint32_t flags;
	uint32_t align;
} elf_program_t;

typedef struct {
	uint32_t name;
	uint32_t value;
	uint32_t size;
	uint8_t info;
	uint8_t other;
	uint16_t shndx;
} elf_sym_t;

int elf_isvalid(elf_header_t* hdr);
