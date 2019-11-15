/** \file arch/x86/include/newland/arch/multiboot.h
 * \author Tristan Ross
 * \copyright GNU Public License 3.0
 * \brief x86 multiboot functions
 * \since v0.1.0
 */
#pragma once

#include <newland/boot/multiboot.h>

/**
 * Creates devices from the multiboot modules
 *
 * @param[in] mbi The multiboot header of the devices
 * @return Zero on success or a negative errno code
 */
int mbi_modules_init(multiboot_info_t* mbi);
