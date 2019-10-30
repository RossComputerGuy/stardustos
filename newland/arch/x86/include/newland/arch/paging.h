/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#pragma once

#include <newland/types.h>

typedef struct {
  uint32_t present:1;
  uint32_t rw:1;
  uint32_t user:1;
  uint32_t accessed:1;
  uint32_t dirty:1;
  uint32_t unused:7;
  uint32_t frame:20;
} page_t;

typedef struct {
  page_t pages[1024];
} page_table_t;

typedef struct {
  page_table_t* tables[1024];
  uint32_t tables_phys[1024];
  uint32_t phys_addr;
} page_dir_t;

int page_alloc_frame(page_t* page, int iskernel, int iswritable);
void page_free_frame(page_t* page);
page_t* paging_get_page(uint32_t address, int make, page_dir_t* dir);
void paging_switch_dir(page_dir_t* dir);
void paging_init();