#ifndef BIFIT_COMMON_TYPES_EXECUTION_H_
#define BIFIT_COMMON_TYPES_EXECUTION_H_

#include "../bifit_common_stack.h"
#include "class_types.h"

typedef struct bifit_local_variable {

    bifit_object_reference_t *object_reference;

} bifit_local_variable_t;

typedef struct bifit_operand {

    bifit_object_reference_t *object_reference;

} bifit_operand_t;

typedef struct bifit_stack_frame {

    bifit_class_t *current_class;
    bifit_method_t *current_method;
    bifit_stack_t operand_stack;
    bifit_local_variable_t *local_variable_array;

} bifit_stack_frame_t;

typedef struct bifit_resolved_static_reference_node {

    bifit_identifier_t *class_identifier;
    bifit_identifier_t *field_identifier;
    bifit_object_reference_t *reference;
    struct bifit_resolved_static_reference_node *next;

} bifit_resolved_static_reference_node_t;

typedef struct bifit_context {

    bifit_class_t *class_list;
    unsigned int class_list_size;
    bifit_resolved_static_reference_node_t *resolved_static_references;
    bifit_stack_t frame_stack;

} bifit_context_t;

typedef struct bifit_object {

    bifit_object_reference_t reference;
    bifit_class_t *object_class;

    void *meta;

} bifit_object_t;

#endif