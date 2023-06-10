#ifndef BIFIT_COMMON_DEFS_H_
#define BIFIT_COMMON_DEFS_H_

#define LOG_DEBUG printf
#define LOG_ERROR printf
#define KERNEL_PANIC(msg) printf("PANIC: %s\n\nsystem will shut down.\n", msg); exit(1);

#endif