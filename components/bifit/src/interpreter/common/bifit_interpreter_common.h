#ifndef BIFIT_INTERPRETER_COMMON_H_
#define BIFIT_INTERPRETER_COMMON_H_

#include "bifit_operand_stack.h"

void bifit_interpreter_execute_current_stack_frame(bifit_context_t *context);

bifit_stack_frame_t *bifit_interpreter_allocate_stack_frame(bifit_context_t *context, bifit_class_t *clazz, bifit_method_t *method) {

    bifit_stack_frame_t *new_stack_frame = malloc(sizeof(struct bifit_stack_frame));
    BIFIT_DEBUG_GC("malloc 12 %p\n", new_stack_frame);

    new_stack_frame->current_class = clazz;
    new_stack_frame->current_method = method;

    new_stack_frame->operand_stack.top = NULL;
    new_stack_frame->local_variable_array = malloc(
            sizeof(struct bifit_local_variable) * method->code.max_locals
    );
    BIFIT_DEBUG_GC("malloc 13 %p\n", new_stack_frame->local_variable_array);

    BIFIT_LOG_DEBUG("allocation done - pushing frame onto stack\n");
    bifit_stack_push(&(context->frame_stack), new_stack_frame);


    return new_stack_frame;
}

void bifit_interpreter_free_stack_frame(bifit_stack_frame_t *stack_frame) {

    BIFIT_DEBUG_GC("free 13 %p\n", stack_frame->local_variable_array);
    free(stack_frame->local_variable_array);
    
    while (stack_frame->operand_stack.top != NULL) {
        BIFIT_DEBUG_GC("deallocate remaining opstack\n");
        bifit_operand_stack_pop_reference(&(stack_frame->operand_stack));
    }

    BIFIT_DEBUG_GC("free 12 %p\n", stack_frame);
    free(stack_frame);
}

void bifit_interpreter_deallocate_current_stack_frame(bifit_context_t *context) {
    bifit_interpreter_free_stack_frame(
            bifit_stack_pop(
                    &(context->frame_stack)
            )
    );
}

/**
 * Parse a method descriptor in order to get the number of values
 * on the operand stack which should be passes to the called method
 * as arguments
 * 
 * @param method_descriptor
 *  i.e.: (Ljava/lang/Class;[Ljava/lang/String;)V
 * @return
 *  the number of arguments as uint8_t
 */
uint8_t bifit_interpreter_count_arguments(bifit_identifier_t *method_descriptor) {
    uint8_t result = 0;
    // start at 1 as first character is a '('
    for (int i = 1; i < method_descriptor->identifier_length; ++i) {

        uint8_t current_char = method_descriptor->identifier[i];

        if (current_char == ')') {
            break;
        }
        
        // we assume every argument is ended by a ';'
        if (current_char == ';') {
            result++;
        }
    }
    return result;
}

#endif