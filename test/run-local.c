#define BIFIT_CONFIG_RUN_LOCAL
#define BIFIT_CONFIG_DONT_DIE_ON_UNKNOWN_OPCODE
#define BIFIT_CONFIG_ENABLE_DEBUG_LOGGING

#include "../components/bifit/include/bifit.h"

int main() {
    bifit_run();
}