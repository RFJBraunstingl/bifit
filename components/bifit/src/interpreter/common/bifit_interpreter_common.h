#ifndef BIFIT_INTERPRETER_COMMON_H_
#define BIFIT_INTERPRETER_COMMON_H_

#include "bifit_operand_stack.h"

void bifit_interpreter_execute_current_stack_frame(bifit_context_t *context);

bifit_stack_frame_t *
bifit_interpreter_allocate_stack_frame(bifit_context_t *context, bifit_class_t *clazz, bifit_method_t *method) {

    bifit_stack_frame_t *new_stack_frame = malloc(sizeof(struct bifit_stack_frame));
    new_stack_frame->current_class = clazz;
    new_stack_frame->current_method = method;

    new_stack_frame->operand_stack.top = NULL;
    new_stack_frame->local_variable_array = malloc(
            sizeof(struct bifit_local_variable) * method->code.max_locals
    );

    BIFIT_LOG_DEBUG("allocation done - pushing frame onto stack\n");
    bifit_stack_push(&(context->frame_stack), new_stack_frame);


    return new_stack_frame;
}

void bifit_interpreter_free_stack_frame(bifit_stack_frame_t *stack_frame) {

    free(stack_frame->local_variable_array);
    free(stack_frame);
}

void bifit_interpreter_deallocate_current_stack_frame(bifit_context_t *context) {
    bifit_interpreter_free_stack_frame(
            bifit_stack_pop(
                    &(context->frame_stack)
            )
    );
}

#endif