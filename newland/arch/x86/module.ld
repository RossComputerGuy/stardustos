SECTIONS {
  . = 1M;
  __module_start = .;

  .text BLOCK(4K) : ALIGN(4K) {
    *(.text)
  }

  .rodata BLOCK(4K) : ALIGN(4K) {
    *(.rodata)
  }

  .modinfo BLOCK(4K) : ALIGN(4K) {
    __modules_start = .;
    *(.modinfo)
    __modules_end = .;
  }

  .bss BLOCK(4K) : ALIGN(4K) {
    *(COMMON)
    *(.bss)
  }

  __module_end = .;
}
