/**
 * StardustOS libfile - (C) 2019 Tristan Ross
 */
#include <libfile/elf.h>

int elf_isvalid(elf_header_t* hdr) {
	char* magic = (char*)&hdr->ident;
	return magic[0] == ELFMAG0 && magic[1] == ELFMAG1 && magic[2] == ELFMAG2 && magic[3] == ELFMAG3 && magic[4] == 1 && magic[5] == 1 && (hdr->type == ELF_REL || hdr->type == ELF_EXEC || hdr->type == ELF_SHAR) && hdr->version == 1;
}
