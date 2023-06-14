//
// Created by Roman Braunstingl on 10.06.23.
//

#ifndef BIFIT_BIFIT_OBJECT_H
#define BIFIT_BIFIT_OBJECT_H

#include "bifit_object_common.h"
#include "bifit_gc.h"

#define OBJECT_CREATIONS_UNTIL_GC 50

bifit_object_t *bifit_object_create() {
    bifit_object_t *new_object = malloc(sizeof(struct bifit_object));
    if (new_object == NULL) {
        // if there is no more heap space, make one attempt at fixing this via gc
        bifit_gc_run();

        new_object = malloc(sizeof(struct bifit_object));
        if (new_object == NULL) {
            BIFIT_KERNEL_PANIC("ERROR: OutOfMemory")
        }
    }

    // init reference
    new_object->reference.id = (++bifit_object_counter);
    new_object->meta = NULL;
    if (bifit_object_counter % OBJECT_CREATIONS_UNTIL_GC == 0) {
        bifit_gc_run();
    }

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

bifit_object_t *bifit_object_get(bifit_object_reference_t *reference) {
    bifit_object_node_t *p = bifit_object_register;
    while (p != NULL) {
        if (bifit_object_cmp_reference(
                &(p->object->reference),
                reference
        ) == 0) {

            return p->object;
        }

        p = p->next;
    }

    return NULL;
}

#endif //BIFIT_BIFIT_OBJECT_H
