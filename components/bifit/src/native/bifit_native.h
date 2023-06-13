//
// Created by Roman Braunstingl on 10.06.23.
//

#ifndef BIFIT_BIFIT_NATIVE_H
#define BIFIT_BIFIT_NATIVE_H

#include "bifit_native_common.h"
#include "./classes/bifit_native_classes.h"

static bifit_object_t *system_out;

void bifit_native_bind(bifit_context_t *context) {

    /* static fields */
    // System.out
    system_out = bifit_object_create();
    bifit_resolved_static_reference_put(
            context,
            bifit_identifier_from_string("java/lang/System", 16),
            bifit_identifier_from_string("out", 3),
            &(system_out->reference)
    );

    /* native classes */
    bifit_native_classes_bind();
}

bool bifit_native_is_native_class(bifit_identifier_t *class_identifier) {
    return bifit_native_classes_get(class_identifier) != NULL;
}

bool bifit_native_execute_method(
        bifit_identifier_t *class_identifier,
        bifit_identifier_t *method_identifier,
        bifit_context_t *context) {

    bifit_native_class_t *native_class = bifit_native_classes_get(class_identifier);
    if (native_class == NULL) {
        return false;
    }

    bifit_native_method_node_t *p = native_class->methods;
    while (p != NULL) {
        bifit_native_method_t *method = p->method;
        if (bifit_identifier_matches_identifier(method_identifier, method->name)) {
            method->impl(context);
            return true;
        }

        p = p->next;
    }

    return false;
}

#endif //BIFIT_BIFIT_NATIVE_H
