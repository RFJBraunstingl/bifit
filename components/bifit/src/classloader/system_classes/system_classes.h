#ifndef BIFIT_SYSTEM_CLASSES_H_
#define BIFIT_SYSTEM_CLASSES_H_

#include "../../common/bifit_common.h"

void java_lang_System(bifit_class_t *out) {
    int num_of_fields = 1;
    out->fields.field_array = malloc(sizeof(struct bifit_field) * num_of_fields);

    // out->fields.field_array
}

#endif