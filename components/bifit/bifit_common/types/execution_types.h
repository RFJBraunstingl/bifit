#ifndef BIFIT_COMMON_TYPES_EXECUTION_H_
#define BIFIT_COMMON_TYPES_EXECUTION_H_

#include "class_types.h"

typedef struct bifit_local_variable {

    struct bifit_local_variable *next;

} bifit_local_variable_t;

typedef struct bifit_operand_stack_element {

    bifit_object_reference_t *object_reference;
    struct bifit_operand_stack_element *next;

} bifit_operand_stack_element_t;

typedef struct bifit_operand_stack {

    bifit_operand_stack_element_t *top;
    bifit_operand_stack_element_t *bottom;

} bifit_operand_stack_t;

typedef struct bifit_context {

    bifit_class_t *class_list;
    unsigned int class_list_size;

} bifit_context_t;

typedef struct bifit_stack_frame {

    bifit_class_t *current_class;
    bifit_method_t *current_method;
    bifit_local_variable_t *local_variable_head;
    bifit_operand_stack_t operand_stack;
    bifit_context_t *bifit_context;

} bifit_stack_frame_t;

typedef struct bifit_object {

    bifit_object_reference_t reference;
    bifit_class_t *object_class;

} bifit_object_t;

#endif