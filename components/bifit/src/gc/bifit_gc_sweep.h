//
// Created by Roman Braunstingl on 13.06.23.
//

#ifndef BIFIT_BIFIT_GC_SWEEP_H
#define BIFIT_BIFIT_GC_SWEEP_H

#include "bifit_gc_common.h"

void bifit_gc_sweep() {
    bifit_object_node_t *p = bifit_object_register;
    while (p != NULL) {
        bifit_object_node_t *next = p->next;

        if (!p->marked) {
            bifit_object_node_destroy(p);
        }

        p = next;
    }
}

#endif //BIFIT_BIFIT_GC_SWEEP_H
