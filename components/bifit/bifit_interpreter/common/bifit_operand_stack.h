#ifndef BIFIT_INTERPRETER_COMMON_OPERAND_STACK_H_
#define BIFIT_INTERPRETER_COMMON_OPERAND_STACK_H_

#include "../../bifit_common/bifit_common.h"

bifit_stack_element_t *bifit_operand_stack_create_element(bifit_object_reference_t *reference) {

    bifit_operand_t *operand = malloc(sizeof(struct bifit_operand));
    operand->object_reference = reference;

    bifit_stack_element_t *stack_element = bifit_stack_create_element();
    stack_element->data = operand;

    return stack_element;
}

void bifit_push_reference_onto_operand_stack(bifit_object_reference_t *reference,
                                             bifit_stack_t *operand_stack) {

    LOG_DEBUG("bifit_push_reference_onto_operand_stack allocating new stack element\n");
    bifit_stack_element_t *new_stack_element = bifit_operand_stack_create_element(reference);

    LOG_DEBUG("allocate success\n");
    bifit_stack_push(operand_stack, new_stack_element);
}

#endif