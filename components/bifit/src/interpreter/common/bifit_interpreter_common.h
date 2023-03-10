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

bifit_stack_frame_t *bifit_allocate_stack_frame(bifit_context_t *context, bifit_class_t *clazz, bifit_method_t *method) {

    bifit_stack_frame_t *new_stack_frame = malloc(sizeof(struct bifit_stack_frame));
    new_stack_frame->current_class = clazz;
    new_stack_frame->current_method = method;

    new_stack_frame->operand_stack.top = NULL;
    new_stack_frame->local_variable_array = malloc(
            sizeof(struct bifit_local_variable) * method->code.max_locals
    );

    bifit_stack_element_t *new_stack_element = bifit_stack_create_element_with_data(new_stack_frame);
    LOG_DEBUG("allocation done - pushing frame onto stack\n");
    bifit_stack_push(&(context->frame_stack), new_stack_element);


    return new_stack_frame;
}

#endif