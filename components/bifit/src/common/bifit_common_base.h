#ifndef BIFIT_COMMON_BASE_H_
#define BIFIT_COMMON_BASE_H_

#define LOG_DEBUG printf
#define LOG_ERROR printf
#define KERNEL_PANIC(msg) printf("PANIC: %s\n\nsystem will shut down.\n", msg); exit(1);

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "types/bifit_types.h"

#endif