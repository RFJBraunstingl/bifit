#ifndef BIFIT_INTERPRETER_COMMON_H_
#define BIFIT_INTERPRETER_COMMON_H_

#include "../bifit_common/bifit_common.h"

static unsigned int object_reference_counter = 0;

bifit_object_t *bifit_create_object() {
    bifit_object_t *new_object = malloc(sizeof(struct bifit_object));

    // init reference
    new_object->reference.id = (++object_reference_counter);

    return new_object;
}

void bifit_push_reference_onto_operand_stack(bifit_object_reference_t *reference,
                                     bifit_operand_stack_t *operand_stack) {

    LOG_DEBUG("bifit_push_reference_onto_operand_stack allocating new stack element\n");
    bifit_operand_stack_element_t *new_stack_element =
            malloc(sizeof(struct bifit_operand_stack_element));
    new_stack_element->object_reference = reference;

    LOG_DEBUG("allocate success - push element on top...\n");

    if (operand_stack->top == NULL) {
        LOG_DEBUG("stack was empty - element is top\n");
        operand_stack->top = new_stack_element;
        operand_stack->bottom = new_stack_element;
    } else {
        operand_stack->top->next = new_stack_element;
        operand_stack->top = new_stack_element;
    }
}

#endif