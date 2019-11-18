/** \file include/newland/syscall.h
 * \author Tristan Ross
 * \copyright GNU Public License 3.0
 * \brief Kernel system calls
 * \since v0.1.0
 */
#pragma once

typedef int (*syscall_t)(void* arg0, void* arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6);

/**
 * Exectes a system call
 *
 * @param[in] i The system call to execute
 * @param arg0,arg1,arg2,arg3,arg4,arg5,arg6 Arguments for the system call
 * @return Anything positive but will be a negative errno on failure
 */
int syscall_run(int i, void* arg0, void* arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6);