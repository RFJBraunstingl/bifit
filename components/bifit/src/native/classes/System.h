//
// Created by Roman Braunstingl on 10.06.23.
//

#ifndef BIFIT_NATIVE_CLASS_SYSTEM_H
#define BIFIT_NATIVE_CLASS_SYSTEM_H

#include "../bifit_native_common.h"

bifit_native_class_t *bifit_native_class_System_init() {
    bifit_identifier_t *identifier = bifit_identifier_from_string(
            "java/lang/System",
            16
    );

    bifit_native_class_t *clazz = malloc(sizeof(struct bifit_native_class));
    BIFIT_DEBUG_GC("malloc 21 %p\n", clazz);
    clazz->identifier = identifier;
    clazz->methods = NULL;

    return clazz;
}

#endif //BIFIT_NATIVE_CLASS_SYSTEM_H
