SECTIONS {
  . = 1M;
  __module_start = .;

  .text BLOCK(4K) : ALIGN(4K) {
    *(.text)
  }

  .rodata BLOCK(4K) : ALIGN(4K) {
    *(.rodata)
  }
  
  .data BLOCK(4K) : ALIGN(4K) {
    __data_start = .;
		*(.data)
    __data_end = .;
    __modules_start = .;
    KEEP(*(modinfo*))
    __modules_end = .;
  }

  .bss BLOCK(4K) : ALIGN(4K) {
    *(COMMON)
    *(.bss)
  }

  __module_end = .;
}
