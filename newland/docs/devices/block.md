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

[register_blkdev]: https://github.com/RossComputerGuy/stardustos/blob/master/#L
[unregister_blkdev]: https://github.com/RossComputerGuy/stardustos/blob/master/#L
