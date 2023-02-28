#ifndef BIFIT_INTERPRETER_COMMON_H_
#define BIFIT_INTERPRETER_COMMON_H_

#include "bifit_operand_stack.h"

void bifit_execute_current_stack_frame_in_context(bifit_context_t *context);

static unsigned int object_reference_counter = 0;

bifit_object_t *bifit_create_object() {
    bifit_object_t *new_object = malloc(sizeof(struct bifit_object));

    // init reference
    new_object->reference.id = (++object_reference_counter);

    return new_object;
}

#endif