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

bool bifit_native_is_native_class(bifit_identifier_t *class_identifier) {
    return bifit_identifier_matches_string(class_identifier, "java/lang/System") ||
           bifit_identifier_matches_string(class_identifier, "java/io/PrintStream");
}

bool bifit_native_execute_method(
        bifit_identifier_t *class_identifier,
        bifit_identifier_t *method_identifier) {

    if (!bifit_native_is_native_class(class_identifier)) {
        return false;
    }


}

#endif //BIFIT_BIFIT_NATIVE_H
