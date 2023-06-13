//
// Created by Roman Braunstingl on 13.06.23.
//

#ifndef BIFIT_BIFIT_OBJECT_COMMON_H
#define BIFIT_BIFIT_OBJECT_COMMON_H

#include "../common/bifit_common_base.h"

typedef struct bifit_object_node {

    bifit_object_t *object;
    struct bifit_object_node *next;
    struct bifit_object_node *prev;
    bool marked;

} bifit_object_node_t;

static unsigned int bifit_object_counter = 0;
static bifit_object_node_t *bifit_object_register = NULL;

#endif //BIFIT_BIFIT_OBJECT_COMMON_H
