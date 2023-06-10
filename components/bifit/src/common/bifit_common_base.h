#ifndef BIFIT_COMMON_BASE_H_
#define BIFIT_COMMON_BASE_H_

#ifdef ENABLE_DEBUG_LOGGING
#define LOG_DEBUG printf
#else
void bifit_log_debug_printf_stub(const char *str, ...) {}
#define LOG_DEBUG bifit_log_debug_printf_stub
#endif
#define LOG_ERROR printf
#define KERNEL_PANIC(msg) printf("PANIC: %s\n\nsystem will shut down.\n", msg); exit(1);

#define CONSTANT_POOL_TAG_UTF8 1
#define CONSTANT_POOL_TAG_INTEGER 3
#define CONSTANT_POOL_TAG_FLOAT 4
#define CONSTANT_POOL_TAG_LONG 5
#define CONSTANT_POOL_TAG_DOUBLE 6
#define CONSTANT_POOL_TAG_CLASS 7
#define CONSTANT_POOL_TAG_STRING 8
#define CONSTANT_POOL_TAG_FIELDREF 9
#define CONSTANT_POOL_TAG_METHOD_REF 10
#define CONSTANT_POOL_TAG_INTERFACE_METHOD_REF 11
#define CONSTANT_POOL_TAG_NAME_AND_TYPE 12
#define CONSTANT_POOL_TAG_METHOD_HANDLE 15
#define CONSTANT_POOL_TAG_METHOD_TYPE 16
#define CONSTANT_POOL_TAG_DYNAMIC 17
#define CONSTANT_POOL_TAG_INVOKE_DYNAMIC 18
#define CONSTANT_POOL_TAG_MODULE 19
#define CONSTANT_POOL_TAG_PACKAGE 20

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "types/bifit_types.h"

#endif