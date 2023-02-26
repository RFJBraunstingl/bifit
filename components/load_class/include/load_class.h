#include "bifit_types.h"

#include "../class_header.h"
#include "../constant_pool.h"
#include "../class_access_flags.h"
#include "../class_identifier.h"
#include "../interfaces.h"
#include "../fields.h"
#include "../methods.h"

/* prototypes */
bifit_class_t *bifit_load_embedded_classes();
void bifit_load_class(const uint8_t *data, bifit_class_t *out);