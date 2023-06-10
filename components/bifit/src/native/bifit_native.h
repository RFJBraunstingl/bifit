//
// Created by Roman Braunstingl on 10.06.23.
//

#ifndef BIFIT_BIFIT_NATIVE_H
#define BIFIT_BIFIT_NATIVE_H

#include "../common/bifit_common.h"

static bifit_object_t *system_out;

void bifit_bind_native_references(bifit_context_t *context) {

    // System.out
    uint8_t system_identifier[] = "System";
    bifit_identifier_t *system_class_identifier = malloc(sizeof(struct bifit_identifier));
    system_class_identifier->identifier = system_identifier;
    system_class_identifier->identifier_length = 6;

    uint8_t out_identifier[] = "out";
    bifit_identifier_t *out_field_identifier = malloc(sizeof(struct bifit_identifier));
    system_class_identifier->identifier = out_identifier;
    system_class_identifier->identifier_length = 3;

    system_out = bifit_create_object();

    bifit_resolved_static_reference_put(
            context,
            system_class_identifier,
            out_field_identifier,
            &(system_out->reference)
    );
}

#endif //BIFIT_BIFIT_NATIVE_H
