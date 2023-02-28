#ifndef BIFIT_INTERPRETER_COMMON_OPERAND_STACK_H_
#define BIFIT_INTERPRETER_COMMON_OPERAND_STACK_H_

#include "../../bifit_common/bifit_common.h"

void bifit_operand_stack_push(bifit_operand_stack_t *operand_stack,
                              bifit_operand_stack_element_t *new_element) {

    if (operand_stack->top == NULL) {
        LOG_DEBUG("stack was empty - element is top now\n");
        operand_stack->top = new_element;
        new_element->prev = NULL;
    } else {
        LOG_DEBUG("push element on top...\n");
        operand_stack->top->next = new_element;
        new_element->prev = operand_stack->top;
        operand_stack->top = new_element;
    }
}

bifit_operand_stack_element_t *bifit_operand_stack_pop(bifit_operand_stack_t *operand_stack) {

    LOG_DEBUG("bifit_operand_stack_pop\n");

    if (operand_stack->top == NULL) {
        LOG_DEBUG("ERROR: stack was empty - can not pop\n");
        exit(1);
    } else {
        bifit_operand_stack_element_t *ref = operand_stack->top;
        operand_stack->top = operand_stack->top->prev;
        operand_stack->top = NULL;
        return ref;
    }
}

bifit_operand_stack_element_t *bifit_operand_stack_peek(bifit_operand_stack_t *operand_stack) {
    return operand_stack->top;
}

bifit_operand_stack_element_t *bifit_operand_stack_create_element(bifit_object_reference_t *reference) {

    bifit_operand_stack_element_t *new_stack_element =
            malloc(sizeof(struct bifit_operand_stack_element));
    new_stack_element->object_reference = reference;

    return new_stack_element;
}

bifit_operand_stack_element_t *bifit_operand_stack_duplicate_element(bifit_operand_stack_element_t *element) {
    return bifit_operand_stack_create_element(element->object_reference);
}

void bifit_push_reference_onto_operand_stack(bifit_object_reference_t *reference,
                                             bifit_operand_stack_t *operand_stack) {

    LOG_DEBUG("bifit_push_reference_onto_operand_stack allocating new stack element\n");
    bifit_operand_stack_element_t *new_stack_element = bifit_operand_stack_create_element(reference);

    LOG_DEBUG("allocate success\n");
    bifit_operand_stack_push(operand_stack, new_stack_element);
}

#endif