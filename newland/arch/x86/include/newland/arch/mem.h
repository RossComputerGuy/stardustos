/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#pragma once

#include <newland/boot/multiboot.h>
#include <newland/types.h>

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

typedef struct {
  page_t entries[1024];
} page_table_t;

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

typedef struct {
  page_dir_entry_t entries[1024];
} page_dir_t;

extern void paging_enable();
extern void paging_disable();
extern void paging_loaddir(page_dir_t dir);
extern void paging_invalidate_tlb();

int phys_isused(unsigned int addr, unsigned int count);
void phys_setused(unsigned int addr, unsigned int count);
void phys_setfree(unsigned int addr, unsigned int count);
unsigned int phys_alloc(unsigned int count);

int page_ispresent(page_dir_t* dir, unsigned int vaddr);
int virt_ispresent(page_dir_t* dir, unsigned int vaddr, unsigned int count);
unsigned int virt2phys(page_dir_t* dir, unsigned int vaddr);
int virt_map(page_dir_t* dir, unsigned int vaddr, unsigned int paddr, unsigned int count, int iswrite, int isuser);
void virt_unmap(page_dir_t* dir, unsigned int vaddr, unsigned int count);

void mem_loadmmap(multiboot_info_t* mbi);
void mem_init(multiboot_info_t* mbi);

unsigned int mem_allocident(page_dir_t* dir, unsigned int count, int iswrite, int isuser);
int mem_identmap(page_dir_t* dir, unsigned int addr, unsigned int count);
