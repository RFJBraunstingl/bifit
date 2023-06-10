//
// Created by Roman Braunstingl on 10.06.23.
//

#ifndef BIFIT_PRINTSTREAM_H
#define BIFIT_PRINTSTREAM_H

#include "bifit_native_classes_common.h"

void bifit_native_PrintStream_println(bifit_context_t *context) {
    return;
}

bifit_native_class_t *bifit_native_class_PrintStream_init() {
    bifit_identifier_t *identifier = bifit_identifier_from_string(
            "java/io/PrintStream",
            19
    );

    bifit_native_class_t *clazz = malloc(sizeof(struct bifit_native_class));
    clazz->identifier = identifier;
    clazz->methods = NULL;

    bifit_native_class_register_method(
            clazz,
            bifit_identifier_from_string("println", 7),
            &bifit_native_PrintStream_println
    );

    return clazz;
}

#endif //BIFIT_PRINTSTREAM_H
