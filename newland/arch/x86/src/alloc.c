/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/arch/mem.h>
#include <newland/alloc.h>
#include <liballoc.h>

int liballoc_lock() {
	return 0;
}

int liballoc_unlock() {
	return 0;
}

void* liballoc_alloc(int pages) {
	return (void*)mem_alloc(get_krnlpgdir(), pages * PAGE_SIZE, 1, 0);
}

int liballoc_free(void* ptr, int pages) {
	mem_free(get_krnlpgdir(), (uint32_t)ptr, pages * PAGE_SIZE);
	return 0;
}
