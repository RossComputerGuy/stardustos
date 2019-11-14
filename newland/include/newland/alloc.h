/**
 * \copyright NewLand Kernel - (C) 2019 Tristan Ross
 */
#pragma once

#include <string.h>

/**
 * Allocates memory for the kernel
 *
 * @param[in] size The number of bytes to allocate
 * @return Pointer to the newly allocated address
 */
void* kmalloc(size_t size);

/**
 * Frees allocated memory for the kernel
 *
 * @param[in] ptr The pointer to the memory address to free
 */
void kfree(void* ptr);

/**
 * Resizes allocated memory for the kernel
 *
 * @param[in] ptr The pointer to the memory address to resize
 * @param[in] size The number of bytes to allocate
 * @return Pointer to the newly allocated address
 */
void* krealloc(void* ptr, size_t size);
