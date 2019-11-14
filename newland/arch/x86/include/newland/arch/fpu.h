/**
 * \copyright NewLand Kernel - (C) 2019 Tristan Ross
 */
#pragma once

#include <newland/arch/proc.h>

void fpu_savectx(proc_t* proc);
void fpu_loadctx(proc_t* proc);
void fpu_init();
