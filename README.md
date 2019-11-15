# StardustOS

[![Build Status](https://img.shields.io/travis/com/RossComputerGuy/stardustos?style=for-the-badge)](https://travis-ci.com/RossComputerGuy/stardustos)
[![Code Climate](https://img.shields.io/codeclimate/coverage/RossComputerGuy/stardustos?style=for-the-badge)](https://codeclimate.com/github/RossComputerGuy/stardustos)
[![License](https://img.shields.io/github/license/RossComputerGuy/stardustos?style=for-the-badge)](https://github.com/RossComputerGuy/stardustos/blob/master/LICENSE)
[![Commit Activity](https://img.shields.io/github/commit-activity/w/RossComputerGuy/stardustos?style=for-the-badge)](https://github.com/RossComputerGuy/stardustos/pulse)
[![Stars](https://img.shields.io/github/stars/RossComputerGuy/stardustos?style=for-the-badge)](https://github.com/RossComputerGuy/stardustos/stargazers)
[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/RossComputerGuy/stardustos)

A modern Unix inspired operating system which does not conform to all of POSIX.

## Features

* Preemptive multiprocessing kernel
* Modular kernel
* Multiboot compliant with x86
* Multiuser system
* Zip file initrd's
* Linux-like boot process

## Building

Compiling StardustOS can be done through the build script in `scripts`,
a Docker container, or CMake.

### CMake

The primary build system for StardustOS is CMake,
the script and Docker container both invoke CMake in one way or another.
Dependencies are CMake, GCC, Grub, and Zip.

First, setup the build directory with CMake:

```
$ cmake -S . -B build
-- The C compiler identification is GNU 9.2.0
-- The ASM compiler identification is GNU
-- Found assembler: /usr/sbin/cc
-- Check for working C compiler: /usr/sbin/cc
-- Check for working C compiler: /usr/sbin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
```

This is to be executed inside of a downloaded version of the source code,
it will use `build` as the directory in which to compile StardustOS in.
The minimum CMake version required is `3.15`.

The second step is the acutal build process:

```
$ make -C build
Scanning dependencies of target kernel.elf
[1%] Building C object newland/CMakeFiles/kernel.elf.dir/libc/src/lib.c.o
[2%] Building C object newland/CMakeFiles/kernel.elf.dir/libc/src/libgen/basename.c.o
[3%] Building C object newland/CMakeFiles/kernel.elf.dir/libc/src/libgen/dirname.c.o
[4%] Building C object newland/CMakeFiles/kernel.elf.dir/libc/src/math/arithm.c.o
[5%] Building C object newland/CMakeFiles/kernel.elf.dir/libc/src/math/pow.c.o
```

This will build the kernel, kernel modules, libraries, programs, initrd,
and rootfs. The disk image should be available as `build/rootfs/rootfs.iso`.

### Docker

StardustOS can be compiled from a Docker container, Travis CI does this and
it saves time from figuring out dependencies as its a more automated way.
The only dependencies are Bash and Docker.

First, setup the Docker container:

```
$ ./scripts/docker-init.sh
Sending build context to Docker daemon 590.8kB
Step 1/6 : FROM archlinux/base
latest: Pulling from archlinux/base
```

This will setup a Debian Sid Docker container called `stardustos`
which will have all the required dependencies installed.

Second, build the operating system:

```
$ ./scripts/docker-run.sh
-- The C compiler identification is GNU 9.2.0
-- The ASM compiler identification is GNU
-- Found assembler: /usr/sbin/cc
-- Check for working C compiler: /usr/sbin/cc
-- Check for working C compiler: /usr/sbin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
```

This runs the container which executes the setup of the CMake build directory
and builds the source. The disk image should be available as `build/rootfs/rootfs.iso`.

### Script

The `scripts/build.sh` script is a simple bash script which easily compiles
StardustOS; it just requires Bash, CMake, GCC, Grub, and Zip.
To invoke the build process, run `./scripts/build.sh` inside of a terminal
and the disk image should be available as `build/rootfs/rootfs.iso`.

## FAQ

### Why not include full POSIX compatibility

I decided to have partial/basic POSIX compatibility since
I wanted to have Vim and basic programs to run without issues
but I don't want to be fully POSIX compliant since there's just more work,
it is less fun to implement, and it would be just redesigning Linux/Unix.

### Can I contribute

Yes, I'm happy to accept pull requests as long as I think they
should be added in.
Check [`CONTRIBUTING.md`][contrib] for knowing how I expect adding features or
fixing code should be done. I suggest looking at the issues marked
"help wanted" or ones that have been out with no fix for over a few days
since those tend be to harder to fix.

[contrib]: https://github.com/RossComputerGuy/stardustos/blob/master/CONTRIBUTING.md

### Will there be more architecture supported

Yes but right now I'm focusing on x86 first, until the operating system
is usable under x86 there will be no other CPU architectures added.
