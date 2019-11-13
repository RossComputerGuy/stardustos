ENTRY(_start)
SECTIONS {
	. = 1M;
	__kernel_start = .;

	.text BLOCK(4K) : ALIGN(4K) {
		*(.multiboot)
		*(.text)
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

	.bss BLOCK(4K) : ALIGN(4K) {
		__bss_start = .;
		*(COMMON)
		*(.bss)
		__bss_end = .;
	}

	.modinfo_array : {
		PROVIDE_HIDDEN(__modules_start = .);
		KEEP(*(modinfo*))
		PROVIDE_HIDDEN(__modules_end = .);
	}

	__kernel_end = .;
}
