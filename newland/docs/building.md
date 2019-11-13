# Building

The NewLand kernel is integrated into the Stardust operating
system build process which uses CMake. Libraries like libc,
miniz, and liblist are compiled into the kernel to provide convienence.
Kernel modules are linked into their own section which is
commonly at the end of the binary to help prevent issues with code and
variables that are in the kernel. If the kernel is built with
`-DCMAKE_BUILD_TYPE=Debug`, enabling debug mode in CMake, then the GCC
options `-fdump-rtl-expand` and `-Wl,-Map` are set so RTL Expand files are generated
for building a call map and the linker map is generated. The kernel can
be tested using the `make test` command which runs QEMU with no ramdisk,
this is good if your trying to see if a kernel runs but you don't
want to add a ramdisk.
