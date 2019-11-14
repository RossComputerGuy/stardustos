/** \file arch/x86/include/newland/arch/fpu.h
 * \author Tristan Ross
 * \copyright GNU Public License 3.0
 * \brief x86 floating point unit
 * \since v0.1.0
 */
#pragma once

#include <newland/arch/proc.h>

/**
 * Save the FPU context into a process
 *
 * @param[in] proc The process to use
 */
void fpu_savectx(proc_t* proc);

/**
 * Load the FPU context from a process
 *
 * @param[in] proc The process to use
 */
void fpu_loadctx(proc_t* proc);

/**
 * Initializes the FPU
 */
void fpu_init();
