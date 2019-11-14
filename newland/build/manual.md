# NewLand Kernel

The NewLand kernel is by design a modular monolithic kernel written in C and Assembly.
Information regarding how the kernel functions and the build process will be outlines
in this document known as the kernel manual.
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
# TTY Devices

TTY devices are terminal device drivers for the computer,
pseudo TTY's are registered during the boot process which
act like normal TTY's but emulate the features and run in memory.
Most architectures and platforms will have a TTY driver for their
serial ports which make debugging the kernel and system easy.

## Registering

TTY devices are registed with the [`register_tty`][register_tty] function
which will allocate memory for the structure and registers
it with the central device API. In order for a device to register,
it must meet these requirements:

* The name of the TTY must not have been used to be registered already
* The name parameter cannot be `NULL`
* The system must have memory available to allocate
* The `tty_opts_t` must be set with at least pointers to the `read` and `write` functions

When a TTY is registered, the kernel event system sends the `tty:register`
event. The purpose for sending this event is for kernel modules and
subsystems to react when that device is registered.

## Unregistering

TTY devices are unregistered when they are no longer needed, an example
of this is when unloading a kernel module that provides a TTY. The [`unregister_tty`][unregister_tty]
function takes care of cleaning up the structure and removing it from the list.
The TTY device must have already been registered for this function to work and
the name cannot be `NULL`. The kernel event system sends the `tty:unregister`
event and the TTY will be unregistered in the central device API.

[register_tty]: https://github.com/RossComputerGuy/stardustos/blob/master/newland/src/dev/tty.c#L51
[unregister_tty]: https://github.com/RossComputerGuy/stardustos/blob/master/newland/src/dev/tty.c#L68
# Block Devices

This kind of device represents a storage device such as an SSD, USB drive,
IDE drive, etc. The system must have some sort of storage device in order
for NewLand to work as described in the boot process documentation page.
This kind of device does not have to do with partitions, that is handled
through the block layout device API.

## Registering

Block devices are registered through the [`register_blkdev`][register_blkdev]
function. The block device will also be registered through the central device
API. The function requires some information for registering:

* Block device name which cannot be `NULL`
* The system must have memory available to allocate
* The name of the block device must not have been used to be registered already
* The block size and count

The kernel event system will send the `blkdev:register` event
when a block device is registered.

## Unregistering

A block device is required to be unregistered when it
becomes unavailable. The central device API will have it unregistered too and
the kernel event system sends a `blkdev:unregister` event. The function used is
called [`unregister_blkdev`][unregister_blkdev] and just requires the block device
name.

[register_blkdev]: https://github.com/RossComputerGuy/stardustos/blob/master/newland/src/dev/block.c#L89
[unregister_blkdev]: https://github.com/RossComputerGuy/stardustos/blob/master/newland/src/dev/block.c#L108
