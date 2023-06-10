//
// Created by Roman Braunstingl on 10.06.23.
//

#ifndef BIFIT_BIFIT_NATIVE_CLASSES_COMMON_H
#define BIFIT_BIFIT_NATIVE_CLASSES_COMMON_H

#include "../bifit_native_common.h"

void bifit_native_class_register_method(
        bifit_native_class_t *clazz,
        bifit_identifier_t *method_identifier,
        void (*impl)(bifit_context_t *)
) {
    bifit_native_method_t *method = malloc(sizeof(struct bifit_native_method));
    method->name = method_identifier;
    method->impl = impl;

    bifit_native_method_node_t *node = malloc(sizeof(struct bifit_native_method_node));
    node->method = method;
    node->next = NULL;

    if (clazz->methods == NULL) {
        clazz->methods = node;
        return;
    }

    bifit_native_method_node_t *p = clazz->methods;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = node;
}

static bifit_native_class_node_t *bifit_native_classes = NULL;

void bifit_native_classes_put(bifit_native_class_t *clazz) {
    bifit_native_class_node_t *node = malloc(sizeof(struct bifit_native_class));
    node->clazz = clazz;
    node->next = NULL;

    if (bifit_native_classes == NULL) {
        bifit_native_classes = node;
        return;
    }

    bifit_native_class_node_t *p = bifit_native_classes;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = node;
}

bifit_native_class_t *bifit_native_classes_get(bifit_identifier_t *identifier) {
    bifit_native_class_node_t *p = bifit_native_classes;
    while (p != NULL) {
        if (bifit_identifier_matches_identifier(
                identifier,
                p->clazz->identifier
        )) {
            return p->clazz;
        }

        p = p->next;
    }

    return NULL;
}

#endif //BIFIT_BIFIT_NATIVE_CLASSES_COMMON_H
