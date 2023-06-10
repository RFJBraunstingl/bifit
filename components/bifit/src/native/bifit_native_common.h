//
// Created by Roman Braunstingl on 10.06.23.
//

#ifndef BIFIT_BIFIT_NATIVE_COMMON_H
#define BIFIT_BIFIT_NATIVE_COMMON_H

#include "../common/bifit_common.h"

typedef struct bifit_native_method {

    bifit_identifier_t *name;
    void (*impl)(bifit_context_t*);

} bifit_native_method_t;

typedef struct bifit_native_method_node {

    bifit_native_method_t *method;
    struct bifit_native_method_node *next;

} bifit_native_method_node_t;

typedef struct bifit_native_class {

    bifit_identifier_t *identifier;
    bifit_native_method_node_t *methods;

} bifit_native_class_t;

typedef struct bifit_native_class_node {

    bifit_native_class_t *clazz;
    struct bifit_native_class_node *next;

} bifit_native_class_node_t;

#endif //BIFIT_BIFIT_NATIVE_COMMON_H
