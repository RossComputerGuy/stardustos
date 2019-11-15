/** \file arch/x86/include/newland/arch/timer.h
 * \author Tristan Ross
 * \copyright GNU Public License 3.0
 * \brief x86 programmable interrupt controller
 * \since v0.1.0
 */
#pragma once

#include <sys/types.h>

/**
 * Set the PIC frequency
 *
 * @param[in] freq The frquency
 */
void timer_setfreq(uint32_t freq);

/**
 * @param[in] freq Sets the timer frequency to this value
 */
void timer_init(uint32_t freq);
