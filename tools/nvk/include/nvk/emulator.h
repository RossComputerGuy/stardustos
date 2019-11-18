#pragma once

#include <unicorn/unicorn.h>

uc_err nvk_emu(void* prog, size_t size, void* impl);