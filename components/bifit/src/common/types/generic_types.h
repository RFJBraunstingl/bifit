#ifndef BIFIT_COMMON_TYPES_GENERIC_H_
#define BIFIT_COMMON_TYPES_GENERIC_H_

#include <stdlib.h>

typedef struct bifit_identifier {

    const uint8_t *identifier;
    unsigned int identifier_length;

} bifit_identifier_t;

typedef struct bifit_attribute {

    bifit_identifier_t name;
    unsigned int length_in_bytes;
    const uint8_t *data;

} bifit_attribute_t;

typedef struct bifit_object_reference {

    unsigned int id;

} bifit_object_reference_t;

#endif