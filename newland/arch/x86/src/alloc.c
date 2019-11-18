/**
 * NewLand Kernel - (C) 2019 Tristan Ross
 */
#include <newland/arch/mem.h>
#include <newland/alloc.h>

struct block_meta {
	size_t size;
	struct block_meta* next;
	int free;
};

void* global_base = NULL;

static struct block_meta* find_free_block(struct block_meta** last, size_t size) {
	struct block_meta* curr = global_base;
	while (curr && !(curr->free && curr->size >= size)) {
		*last = curr;
		curr = curr->next;
	}
	return curr;
}

static struct block_meta* req_space(struct block_meta* last, size_t size) {
	page_dir_t* pgdir = get_krnlpgdir();
	struct block_meta* block = (struct block_meta*)mem_alloc(pgdir, sizeof(struct block_meta) + size, 1, 0);
	if (block == NULL) return NULL;
	if (last) last->next = block;
	block->size = size;
	block->next = NULL;
	block->free = 0;
	return block;
}

void* kmalloc(size_t size) {
	struct block_meta* block;
	if (size == 0) return NULL;
	if (!global_base) {
		block = req_space(NULL, size);
		if (block == NULL) return NULL;
		global_base = block;
	} else {
		struct block_meta* last = global_base;
		block = find_free_block(&last, size);
		if (!block) {
			block = req_space(last, size);
			if (!block) return NULL;
		} else {
			block->free = 0;
		}
	}
	return (block + sizeof(struct block_meta));
}

void kfree(void* ptr) {
	if (!ptr) return;
	struct block_meta* block = (struct block_meta*)((uint32_t*)ptr - sizeof(struct block_meta));
	if (block->free) return;
	block->free = 1;
}

void* krealloc(void* ptr, size_t size) {
	if (ptr == NULL) return kmalloc(size);
	struct block_meta* block = (struct block_meta*)((uint32_t*)ptr - sizeof(struct block_meta));
	if (block->size >= size) return ptr;
	void* nptr = kmalloc(size);
	if (nptr == NULL) return NULL;
	memcpy(nptr, ptr, block->size);
	kfree(ptr);
	return nptr;
}
