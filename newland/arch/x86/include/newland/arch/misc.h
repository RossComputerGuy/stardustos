/**
 * \copyright NewLand Kernel - (C) 2019 Tristan Ross
 */
#pragma once

void halt() __attribute__ ((noreturn));
void panic(const char* msg) __attribute__ ((noreturn));