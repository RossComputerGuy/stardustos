/**
	* StardustOS libc - (C) 2019 Tristan Ross
	*/
#pragma once

#define ENOMEM 1
#define ECHILD 2
#define EEXIST 3
#define ENODEV 4
#define ENAMETOOLONG 5
#define EIO 6
#define EINVAL 7
#define EINPROGRESS 8
#define EINTR 9
#define ENOSYS 10
#define EOVERFLOW 11
#define EISDIR 12
#define EBUSY 13
#define ENOENT 14

extern int errno;
