/** \file include/newland/alloc.h
 * \author Tristan Ross
 * \copyright GNU Public License 3.0
 * \brief Kernel memory allocation
 * \since v0.1.0
 * \details Kernelspace memory allocation functions; includes kmalloc, kfree, and krealloc
 */
#pragma once

#include <string.h>

#ifndef __nvk__
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
#else
#include <stdlib.h>

#define kmalloc malloc
#define kfree free
#define krealloc realloc
#endif
