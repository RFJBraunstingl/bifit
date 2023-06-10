//
// Created by Roman Braunstingl on 10.06.23.
//

#ifndef BIFIT_BIFIT_NATIVE_H
#define BIFIT_BIFIT_NATIVE_H

#include "../common/bifit_common.h"

static bifit_object_t *system_out;

void bifit_native_bind_references(bifit_context_t *context) {

    // System.out
    system_out = bifit_create_object();
    bifit_resolved_static_reference_put(
            context,
            bifit_identifier_from_string("java/lang/System", 16),
            bifit_identifier_from_string("out", 3),
            &(system_out->reference)
    );
}

#endif //BIFIT_BIFIT_NATIVE_H
