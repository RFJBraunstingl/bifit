#ifndef BIFIT_COMMON_TYPES_EXECUTION_H_
#define BIFIT_COMMON_TYPES_EXECUTION_H_

#include "class_types.h"

typedef struct bifit_local_variable {

    struct bifit_local_variable *next;

} bifit_local_variable_t;

typedef struct bifit_context {

    bifit_class_t *class_list;
    unsigned int class_list_size;

} bifit_context_t;

typedef struct bifit_stack_frame {

    bifit_class_t *current_class;
    bifit_method_t *current_method;
    bifit_local_variable_t *local_variable_head;
    bifit_context_t *bifit_context;

} bifit_stack_frame_t;

#endif