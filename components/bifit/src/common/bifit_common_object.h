//
// Created by Roman Braunstingl on 10.06.23.
//

#ifndef BIFIT_BIFIT_COMMON_OBJECT_H
#define BIFIT_BIFIT_COMMON_OBJECT_H

#include "bifit_common_base.h"

static unsigned int object_reference_counter = 0;

bifit_object_t *bifit_create_object() {
    bifit_object_t *new_object = malloc(sizeof(struct bifit_object));

    // init reference
    new_object->reference.id = (++object_reference_counter);

    return new_object;
}

#endif //BIFIT_BIFIT_COMMON_OBJECT_H
