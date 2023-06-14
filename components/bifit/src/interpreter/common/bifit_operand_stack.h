#ifndef BIFIT_INTERPRETER_COMMON_OPERAND_STACK_H_
#define BIFIT_INTERPRETER_COMMON_OPERAND_STACK_H_

#include "../../common/bifit_common.h"

void bifit_operand_stack_push_reference(bifit_stack_t *operand_stack, bifit_object_reference_t *reference) {
    bifit_operand_t *operand = malloc(sizeof(struct bifit_operand));
    BIFIT_DEBUG_GC("malloc 14 %p\n", operand);

    operand->object_reference = reference;

    BIFIT_DEBUG_GC("push operand %p\n", operand);
    bifit_stack_push(operand_stack, operand);
}

bifit_object_reference_t *bifit_operand_stack_pop_reference(bifit_stack_t *operand_stack) {
    bifit_operand_t *operand = bifit_stack_pop(operand_stack);
    BIFIT_DEBUG_GC("pop operand %p\n", operand);
    bifit_object_reference_t *reference = operand->object_reference;
    BIFIT_DEBUG_GC("free 14 %p\n", operand);
    free(operand);
    return reference;
}

#endif