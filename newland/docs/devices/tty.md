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

[register_tty]: https://github.com/RossComputerGuy/stardustos/blob/master/#L
[unregister_tty]: https://github.com/RossComputerGuy/stardustos/blob/master/#L
