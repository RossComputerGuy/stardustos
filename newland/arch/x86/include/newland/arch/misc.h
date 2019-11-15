/** \file arch/x86/include/newland/arch/misc.h
 * \author Tristan Ross
 * \copyright GNU Public License 3.0
 * \brief x86 misc functions
 * \since v0.1.0
 */
#pragma once

/**
 * Halts the CPU
 *
 * @warning The system will not return from this function as it stops the entire system from running
 */
void halt() __attribute__ ((noreturn));

/**
 * Panics the system
 *
 * @warning The system will not return from this function as it stops the entire system from running
 * @param[in] msg The panic message to write into the kernel logger
 */
void panic(const char* msg) __attribute__ ((noreturn));