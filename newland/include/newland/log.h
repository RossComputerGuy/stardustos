/**
 * \copyright NewLand Kernel - (C) 2019 Tristan Ross
 */
#pragma once

#include <string.h>

#define KLOG_SOH "\001"
#define KLOG_SOH_ASCII '\001'

#define KLOG_DEBUG KLOG_SOH "0"
#define KLOG_INFO KLOG_SOH "1" 
#define KLOG_ERR KLOG_SOH "2"
#define KLOG_WARN KLOG_SOH "3"
#define KLOG_FATAL KLOG_SOH "4"
#define KLOG_NOTICE KLOG_SOH "5"

#define KLEVEL_DEBUG 0
#define KLEVEL_INFO 1
#define KLEVEL_ERR 2
#define KLEVEL_WARN 3
#define KLEVEL_FATAL 4
#define KLEVEL_NOTICE 5

int printk(const char* fmt, ...);
