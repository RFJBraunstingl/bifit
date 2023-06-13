//
// Created by Roman Braunstingl on 10.06.23.
//

#ifndef BIFIT_BIFIT_COMMON_OBJECT_H
#define BIFIT_BIFIT_COMMON_OBJECT_H

#include "bifit_common_base.h"

typedef struct bifit_object_node {

    bifit_object_t *object;
    struct bifit_object_node *next;
    struct bifit_object_node *prev;
    bool marked;

} bifit_object_node_t;

static unsigned int bifit_object_counter = 0;
static bifit_object_node_t *bifit_object_register = NULL;

bifit_object_t *bifit_object_create() {
    bifit_object_t *new_object = malloc(sizeof(struct bifit_object));
    if (new_object == NULL) {
        BIFIT_KERNEL_PANIC("ERROR: OutOfMemory")
    }

    // init reference
    new_object->reference.id = (++bifit_object_counter);

    bifit_object_node_t *register_node = malloc(sizeof(struct bifit_object_node));
    register_node->object = new_object;
    register_node->next = bifit_object_register;
    register_node->prev = NULL;

    if (bifit_object_register != NULL) {
        bifit_object_register->prev = register_node;
    }
    bifit_object_register = register_node;

    return new_object;
}

long bifit_object_cmp_reference(
        bifit_object_reference_t *left,
        bifit_object_reference_t *right
) {
    return left->id - right->id;
}

bifit_object_node_t *bifit_object_get_node(bifit_object_reference_t *reference) {
    bifit_object_node_t *p = bifit_object_register;
    while (p != NULL) {
        if (bifit_object_cmp_reference(
                &(p->object->reference),
                reference
        ) == 0) {

            return p;
        }

        p = p->next;
    }

    return NULL;
}

bifit_object_t *bifit_object_get(bifit_object_reference_t *reference) {
    bifit_object_node_t *p = bifit_object_get_node(reference);
    if (p == NULL) {
        return NULL;
    }

    return p->object;
}

void bifit_object_node_destroy(bifit_object_node_t *node) {
    if (node == NULL) {
        return;
    }

    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }

    free(node->object);
    free(node);
}

#endif //BIFIT_BIFIT_COMMON_OBJECT_H
