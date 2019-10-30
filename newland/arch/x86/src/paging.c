/**
  * NewLand Kernel - (C) 2019 Tristan Ross
  */
#include <newland/arch/idt.h>
#include <newland/arch/mem.h>
#include <newland/arch/paging.h>
#include <newland/error.h>
#include <string.h>

#define INDEX_FROM_BIT(a) (a / (8 * 4))
#define OFFSET_FROM_BIT(a) (a % (8 * 4))

static page_dir_t* krn_dir = NULL;
static page_dir_t* curr_dir = NULL;
static uint32_t* frames = NULL;
static uint32_t nframes = 0;
extern uint32_t placement_address;

#define set_frame(addr) frames[INDEX_FROM_BIT((addr) / 0x1000)] |= (0x1 << OFFSET_FROM_BIT((addr) / 0x1000))
#define clear_frame(addr) frames[INDEX_FROM_BIT((addr) / 0x1000)] &= ~(0x1 << OFFSET_FROM_BIT((addr) / 0x1000))
#define test_frame(addr) (frames[INDEX_FROM_BIT((addr) / 0x1000)] & (0x1 << OFFSET_FROM_BIT((addr) / 0x1000)))

static uint32_t first_frame() {
  for (uint32_t i = 0; i < INDEX_FROM_BIT(nframes); i++) {
    if (frames[i] != 0xFFFFFFFF) {
      for (uint32_t j = 0; j < 32; j++) {
        if (!(frames[i] & (0x1 << j))) {
          return i * 4 * 8 + j;
        }
      }
    }
  }
  return -1;
}

int page_alloc_frame(page_t* page, int iskernel, int iswritable) {
  if (page->frame != 0) return 0;
  uint32_t idx = first_frame();
  if (idx == -1) return -ENOMEM;
  set_frame(idx * 0x1000);
  page->present = 1;
  page->rw = iswritable ? 1 : 0;
  page->user = iskernel ? 1 : 0;
  page->frame = idx;
  return 0;
}

void page_free_frame(page_t* page) {
  uint32_t frame = page->frame;
  if (!frame) return;
  clear_frame(frame);
  page->frame = 0;
}

page_t* paging_get_page(uint32_t address, int make, page_dir_t* dir) {
  address /= 0x1000;
  uint32_t table_idx = address / 1024;
  if (dir->tables[table_idx]) return &dir->tables[table_idx]->pages[address % 1024];
  else if(make) {
    uint32_t tmp;
    dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
    memset(dir->tables[table_idx], 0, 0x1000);
    dir->tables_phys[table_idx] = tmp | 0x7;
    return &dir->tables[table_idx]->pages[address % 1024];
  } else return NULL;
}

void paging_switch_dir(page_dir_t* dir) {
  curr_dir = dir;
  asm volatile ("mov %0, %%cr3" :: "r"(&dir->tables_phys));
  uint32_t cr0;
  asm volatile ("mov %%cr0, %0" : "=r"(cr0));
  cr0 |= 0x80000000;
  asm volatile ("mov %0, %%cr0" :: "r"(cr0));
}

void paging_init() {
  uint32_t mem_end_page = 0x1000000;
  nframes = mem_end_page / 0x1000;
  frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes));
  memset(frames, 0, INDEX_FROM_BIT(nframes));

  krn_dir = (page_dir_t*)kmalloc_a(sizeof(page_dir_t));
  memset(krn_dir, 0, sizeof(page_dir_t));
  curr_dir = krn_dir;

  uint32_t i = 0;
  while (i < placement_address) {
    page_alloc_frame(paging_get_page(i, 1, krn_dir), 0, 0);
    i += 0x1000;
  }

  // TODO: register interrupt handler

  paging_switch_dir(krn_dir);
}