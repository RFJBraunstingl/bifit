//
// Created by Roman Braunstingl on 10.06.23.
//

#ifndef BIFIT_NATIVE_CLASS_STRING_H
#define BIFIT_NATIVE_CLASS_STRING_H

#include "bifit_native_classes_common.h"

typedef struct bifit_native_String {

    const uint8_t *str;
    unsigned int len;

} bifit_native_String_t;

bifit_native_String_t *bifit_native_class_String_wrap(const uint8_t *str, size_t len) {
    bifit_native_String_t *result = malloc(sizeof(struct bifit_native_String));
    BIFIT_DEBUG_GC("malloc 19 %p\n", result);
    result->str = str;
    result->len = len;

    return result;
}

bifit_native_class_t *bifit_native_class_String_init() {
    bifit_identifier_t *identifier = bifit_identifier_from_string(
            "java/lang/String",
            16
    );

    bifit_native_class_t *clazz = malloc(sizeof(struct bifit_native_class));
    BIFIT_DEBUG_GC("malloc 20 %p\n", clazz);
    clazz->identifier = identifier;
    clazz->methods = NULL;

    return clazz;
}

#endif //BIFIT_NATIVE_CLASS_STRING_H
