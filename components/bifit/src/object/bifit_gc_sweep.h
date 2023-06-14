//
// Created by Roman Braunstingl on 13.06.23.
//

#ifndef BIFIT_BIFIT_GC_SWEEP_H
#define BIFIT_BIFIT_GC_SWEEP_H

#include "bifit_object.h"

void bifit_object_node_destroy(bifit_object_node_t *node) {
    if (node == NULL) {
        return;
    }

    if (node->prev == NULL) {
        bifit_object_register = node->next;
    } else {
        node->prev->next = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }

    if (node->object->meta != NULL) {
        BIFIT_DEBUG_GC("free 19 %p\n", node->object->meta);
        free(node->object->meta);
    }
    BIFIT_DEBUG_GC("free 22 %p\n", node->object);
    free(node->object);
    BIFIT_DEBUG_GC("free 24 %p\n", node);
    free(node);
}

void bifit_gc_sweep() {
    bifit_object_node_t *p = bifit_object_register;
    while (p != NULL) {
        bifit_object_node_t *next = p->next;

        if (!p->marked) {
            bifit_object_node_destroy(p);
        }

        p = next;
    }

    p = bifit_object_register;
    int counter = 0;
    while (p != NULL)
    {
        ++counter;
        p = p->next;
    }
    printf("after sweeping, %d objects remain\n", counter);    
}

#endif //BIFIT_BIFIT_GC_SWEEP_H
