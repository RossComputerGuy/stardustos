ENTRY(_start)
SECTIONS {
	. = 1M;
  __kernel_start = .;

	.text BLOCK(4K) : ALIGN(4K) {
		*(.multiboot)
		*(.text)
    __text_end = .;
	}

	.rodata BLOCK(4K) : ALIGN(4K) {
    __rodata_start = .;
		*(.rodata)
    __rodata_end = .;
	}

	.data BLOCK(4K) : ALIGN(4K) {
    __data_start = .;
		*(.data)
    __data_end = .;
	}

  .modules BLOCK(4K) : ALIGN(4K) {
    __modules_start = .;
    *(.modinfo)
    __modules_end = .;
  }

	.bss BLOCK(4K) : ALIGN(4K) {
    __bss_start = .;
		*(COMMON)
		*(.bss)
    __bss_end = .;
	}
  __kernel_end = .;
}
