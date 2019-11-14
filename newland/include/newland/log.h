/** \file include/newland/log.h
 * \author Tristan Ross
 * \copyright GNU Public License 3.0
 * \brief Kernel Logging API
 * \since v0.1.0
 * \details Kernel logging using a ringbuffer
 */
#pragma once

#include <string.h>

/**
 * Kernel logging start-of-header
 */
#define KLOG_SOH "\001"

/**
 * Kernel logging start-of-header as a character
 */
#define KLOG_SOH_ASCII '\001'

/**
 * Kernel log level: debug
 */
#define KLOG_DEBUG KLOG_SOH "0"

/**
 * Kernel log level: info
 */
#define KLOG_INFO KLOG_SOH "1" 

/**
 * Kernel log level: error
 */
#define KLOG_ERR KLOG_SOH "2"

/**
 * Kernel log level: warn
 */
#define KLOG_WARN KLOG_SOH "3"

/**
 * Kernel log level: fatal
 */
#define KLOG_FATAL KLOG_SOH "4"

/**
 * Kernel log level: notice
 */
#define KLOG_NOTICE KLOG_SOH "5"

/**
 * Kernel log level: debug
 */
#define KLEVEL_DEBUG 0

/**
 * Kernel log level: info
 */
#define KLEVEL_INFO 1

/**
 * Kernel log level: error
 */
#define KLEVEL_ERR 2

/**
 * Kernel log level: warn
 */
#define KLEVEL_WARN 3

/**
 * Kernel log level: fatal
 */
#define KLEVEL_FATAL 4

/**
 * Kernel log level: notice
 */
#define KLEVEL_NOTICE 5

/**
 * Prints a formatted message to the kernel ringbuffer
 *
 * @param[in] fmt The printf format string
 * @return The number of bytes printed or a negative errno code
 */
int printk(const char* fmt, ...);
