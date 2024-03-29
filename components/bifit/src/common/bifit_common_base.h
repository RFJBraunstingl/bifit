#ifndef BIFIT_COMMON_BASE_H_
#define BIFIT_COMMON_BASE_H_

#ifdef BIFIT_CONFIG_ENABLE_DEBUG_LOGGING
#define BIFIT_LOG_DEBUG printf
#else
void bifit_log_debug_printf_stub(const char *str, ...) {}
#define BIFIT_LOG_DEBUG bifit_log_debug_printf_stub
#endif

#ifdef BIFIT_CONFIG_ENABLE_DEBUG_GC
#define BIFIT_DEBUG_GC printf
#else
void bifit_log_debug_gc_stub(const char *str, ...) {}
#define BIFIT_DEBUG_GC bifit_log_debug_gc_stub
#endif

#ifdef BIFIT_CONFIG_ENABLE_LOG_GC
#define BIFIT_LOG_GC printf
#else
void bifit_log_debug_log_gc_stub(const char *str, ...) {}
#define BIFIT_LOG_GC bifit_log_debug_log_gc_stub
#endif

#define BIFIT_LOG_ERROR printf
#define BIFIT_KERNEL_PANIC(msg) printf("PANIC: %s\n\nsystem will shut down.\n", msg); exit(1);

#define BIFIT_CONSTANT_POOL_TAG_UTF8 1
#define BIFIT_CONSTANT_POOL_TAG_INTEGER 3
#define BIFIT_CONSTANT_POOL_TAG_FLOAT 4
#define BIFIT_CONSTANT_POOL_TAG_LONG 5
#define BIFIT_CONSTANT_POOL_TAG_DOUBLE 6
#define BIFIT_CONSTANT_POOL_TAG_CLASS 7
#define BIFIT_CONSTANT_POOL_TAG_STRING 8
#define BIFIT_CONSTANT_POOL_TAG_FIELDREF 9
#define BIFIT_CONSTANT_POOL_TAG_METHOD_REF 10
#define BIFIT_CONSTANT_POOL_TAG_INTERFACE_METHOD_REF 11
#define BIFIT_CONSTANT_POOL_TAG_NAME_AND_TYPE 12
#define BIFIT_CONSTANT_POOL_TAG_METHOD_HANDLE 15
#define BIFIT_CONSTANT_POOL_TAG_METHOD_TYPE 16
#define BIFIT_CONSTANT_POOL_TAG_DYNAMIC 17
#define BIFIT_CONSTANT_POOL_TAG_INVOKE_DYNAMIC 18
#define BIFIT_CONSTANT_POOL_TAG_MODULE 19
#define BIFIT_CONSTANT_POOL_TAG_PACKAGE 20

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "types/bifit_types.h"

bifit_context_t *bifit_context;

#endif