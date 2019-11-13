# TTY Devices

TTY devices are terminal device drivers for the computer,
pseudo TTY's are registered during the boot process which
act like normal TTY's but emulate the features and run in memory.
Most architectures and platforms will have a TTY driver for their
serial ports which make debugging the kernel and system easy.

## Registering

TTY devices are registed with the [`register_tty` function][register_tty]
which will allocate memory for the structure and registers
it with the central device API. In order for a device to register,
it must meet these requirements:

* The name of the TTY must not have been used to be registered already
* The name parameter cannot be NULL
* The system must have memory available to allocate

* The `tty_opts_t` must be set with at least pointers

to the `read` and `write` functions

[register_tty]: https://github.com/RossComputerGuy/stardustos/blob/master/newland/src/dev/tty.c#L51
