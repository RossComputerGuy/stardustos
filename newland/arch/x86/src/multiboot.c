/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/arch/multiboot.h>
#include <newland/dev/block.h>
#include <newland/dev.h>
#include <newland/errno.h>
#include <stddef.h>
#include <stdlib.h>

static multiboot_info_t* mbi;

/** Normal device opts **/
static size_t mb_read(fs_node_t* node, off_t offset, void* buff, size_t size) {
	uint8_t i = ((node->name[6] - '0') * 10) + (node->name[5] - '0');
	if (i > mbi->mods_count) return -NEWLAND_EINVAL;
	multiboot_module_t* mod = (multiboot_module_t*)mbi->mods_addr;
	for (size_t x = 0; x < i; x++) mod = (multiboot_module_t*)((uint32_t)mod->mod_end + 1);
	if (offset + size >= mod->mod_end) return -NEWLAND_EINVAL;
	size = offset > (mod->mod_end - mod->mod_start) ? 0 : (size_t)min((mod->mod_end - mod->mod_start) - offset, size);
	memcpy(buff, (void*)(mod->mod_start + offset), size);
	return size;
}

static fs_node_opts_t mbimod_opts = { .read = mb_read };

int mbi_modules_init(multiboot_info_t* _mbi) {
	mbi = _mbi;
	multiboot_module_t* mod = (multiboot_module_t*)mbi->mods_addr;
	for (size_t i = 0; i < mbi->mods_count; i++) {
		char name[8];
		strcpy(name, "mbmod");
		itoa(name + 5, 10, i);
		if (name[6] == 0) name[6] = '0';
		name[7] = 0;
		int r = register_device(MKDEV(DEVMAJ_MBMOD, i), name, mbimod_opts, mod->mod_end - mod->mod_start);
		if (r < 0) return r;
		mod = (multiboot_module_t*)((uint32_t)mod->mod_end + 1);
	}
	return 0;
}
