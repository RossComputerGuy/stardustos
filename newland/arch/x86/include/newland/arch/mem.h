/** \file arch/x86/include/newland/arch/mem.h
 * \author Tristan Ross
 * \copyright GNU Public License 3.0
 * \brief x86 memory management
 * \since v0.1.0
 * \details Virtual and physical memory management using paging
 */
#pragma once

#include <newland/boot/multiboot.h>
#include <sys/types.h>

/**
 * Size in bytes of one page
 */
#define PAGE_SIZE 0x1000

/**
 * Aligns upward an integer to the page size
 */
#define PAGE_ALIGN_UP(__x) ((__x) + PAGE_SIZE - ((__x) % PAGE_SIZE))

/**
 * Page
 */
typedef union {
	struct {
		int present:1;
		int write:1;
		int user:1;
		int write_through:1;
		int disable_cache:1;
		int accessed:1;
		int dirty:1;
		int pat:1;
		uint32_t ignored:4;
		uint32_t frame:20;
	};
	uint32_t value;
} page_t;

/**
 * Page table
 */
typedef struct {
	page_t entries[1024];
} page_table_t;

/**
 * Page directory entry
 */
typedef union {
	struct {
		int present:1;
		int write:1;
		int user:1;
		int write_through:1;
		int disable_cache:1;
		int accessed:1;
		int ignored1:1;
		int large_page:1;
		uint32_t ignored2:4;
		uint32_t frame:20;
	};
	uint32_t value;
} page_dir_entry_t;

/**
 * Page directory
 */
typedef struct {
	page_dir_entry_t entries[1024];
} page_dir_t;

/**
 * Enable paging
 */
extern void paging_enable();

/**
 * Disable paging
 */
extern void paging_disable();

/**
 * Load the page directory
 */
extern void paging_loaddir(page_dir_t* dir);

/**
 * Invalidate the page table
 */
extern void paging_invalidate_tlb();

/**
 * Check if the phsyical address is in use
 *
 * @param[in] addr The address to check
 * @param[in] count The address count to check
 * @return True if the physical address is in use right now
 */
int phys_isused(unsigned int addr, unsigned int count);

/**
 * Set the physical address to be in use
 *
 * @param[in] addr The physical address
 * @param[in] count The number of bytes to scan through
 */
void phys_setused(unsigned int addr, unsigned int count);

/**
 * Set the physical address to be free
 *
 * @param[in] addr The physical address
 * @param[in] count The number of bytes to scan through
 */
void phys_setfree(unsigned int addr, unsigned int count);

/**
 * Allocates a physical address
 *
 * @param[in] count The number of bytes to allocate
 * @return A physical memory address
 */
unsigned int phys_alloc(unsigned int count);

/**
 * Check if the page is present
 *
 * @param[in] dir The page directory to check for the page
 * @param[in] vaddr The virtual address of the page
 * @return True if the page is present
 */
int page_ispresent(page_dir_t* dir, unsigned int vaddr);

/**
 * Check if the virtual memory address is present
 *
 * @param[in] dir The page directory to check for the page
 * @param[in] vaddr The virtual address of the page
 * @param[in] count The number of bytes to check through
 * @return True if the virtual memory address is present
 */
int virt_ispresent(page_dir_t* dir, unsigned int vaddr, unsigned int count);

/**
 * Converts a virtual memory address to a physical memory address
 *
 * @param[in] dir The page directory to use
 * @param[in] vaddr The virtual memory address
 * @return The physical memory address
 */
unsigned int virt2phys(page_dir_t* dir, unsigned int vaddr);

/**
 * Maps a memory address
 *
 * @param[in] dir The page directory to map in
 * @param[in] vaddr The virtual memory address to map
 * @param[in] paddr The physical memory address to map
 * @param[in] count The number of bytes to map starting from the addresses
 * @param[in] iswrite Set to true for setting the address as writable
 * @param[in] isuser Set to true to use the mapped address in user mode
 * @return Zero on success or a negative errno code on failure
 */
int virt_map(page_dir_t* dir, unsigned int vaddr, unsigned int paddr, unsigned int count, int iswrite, int isuser);

/**
 * Unmaps a memory address
 *
 * @param[in] dir The page directory the address was mapped in
 * @param[in] vaddr The virtual memory address to unmap
 * @param[in] count The number of bytes to unmap
 */
void virt_unmap(page_dir_t* dir, unsigned int vaddr, unsigned int count);

/**
 * Load the memory map from the multiboot info
 * 
 * @param[in] mbi The mutliboot info structure
 */
void mem_loadmmap(multiboot_info_t* mbi);

/**
 * Initializes the memory manager
 *
 * @param[in] mbi The multiboot info structure
 */
void mem_init(multiboot_info_t* mbi);

/**
 * Allocates an identified memory address
 *
 * @param[in] dir The page directory
 * @param[in] count The nunber of bytes to allocate
 * @param[in] iswrite Set to true for setting the address as writable
 * @param[in] isuser Set to true for setting the address as usable in user mode
 * @return A memory address
 */
unsigned int mem_allocident(page_dir_t* dir, unsigned int count, int iswrite, int isuser);

/**
 * Allocates a memory address
 *
 * @param[in] dir The page directory
 * @param[in] count The nunber of bytes to allocate
 * @param[in] iswrite Set to true for setting the address as writable
 * @param[in] isuser Set to true for setting the address as usable in user mode
 * @return A memory address
 */
unsigned int mem_alloc(page_dir_t* dir, unsigned int count, int iswrite, int isuser);

/**
 * Frees a memory address
 *
 * @param[in] dir The page directory
 * @param[in] addr The address to free
 * @param[in] count The number of bytes to free
 */
void mem_free(page_dir_t* dir, unsigned int addr, unsigned int count);

/**
 * Identify the map
 *
 * @param[in] dir The page directory
 * @param[in] addr The address to identify
 * @param[in] count The number of bytes to identify
 * @return Zero on success or a negative errno code
 */
int mem_identmap(page_dir_t* dir, unsigned int addr, unsigned int count);

/**
 * Maps a page
 *
 * @param[in] pgdir The page directory
 * @param[in] addr The address to map
 * @param[in] count The number of bytes to map
 * @param[in] iswrite Set to true for setting the address as writable
 * @param[in] isuser Set to true for setting the address as usable in user mode
 * @return Zero on success or a negative errno code
 */
int mem_map(page_dir_t* pgdir, unsigned int addr, unsigned int count, int iswrite, int isuser);

/**
 * Get the kernel's page directory
 *
 * @warning This is used by the kernel, do not touch this if you don't know what your doing
 * @return A page directory
 */
page_dir_t* get_krnlpgdir();

/**
 * Allocates a new page directory
 *
 * @return A page directory
 */
page_dir_t* mem_alloc_pgdir();

/**
 * Frees the page directory
 *
 * @param[in] dir The page directory
 */
void mem_free_pgdir(page_dir_t* dir);
