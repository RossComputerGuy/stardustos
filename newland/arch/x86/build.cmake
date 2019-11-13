set(ARCH_FILES "${ARCH_DIR}/modules/bus/pci.c"
	"${ARCH_DIR}/modules/tty/serial.c"
	"${ARCH_DIR}/src/asm/entry.s"
	"${ARCH_DIR}/src/asm/gdt.s"
	"${ARCH_DIR}/src/asm/idt.s"
	"${ARCH_DIR}/src/asm/irq.s"
	"${ARCH_DIR}/src/asm/paging.s"
	"${ARCH_DIR}/src/bootstrap.c"
	"${ARCH_DIR}/src/fpu.c"
	"${ARCH_DIR}/src/gdt.c"
	"${ARCH_DIR}/src/idt.c"
	"${ARCH_DIR}/src/io.c"
	"${ARCH_DIR}/src/irq.c"
	"${ARCH_DIR}/src/mem.c"
	"${ARCH_DIR}/src/misc.c"
	"${ARCH_DIR}/src/multiboot.c"
	"${ARCH_DIR}/src/proc.c"
	"${ARCH_DIR}/src/timer.c")
include_directories("${ARCH_DIR}/include")
set(CMAKE_EXE_LINKER_FLAGS "-m32")
add_compile_options("-m32")
set(EMULATOR i386)
