#include "../common/bifit_interpreter_common.h"

unsigned int bifit_execute_instruction_areturn(
        unsigned int pc,
        bifit_stack_frame_t *stack_frame,
        bifit_context_t *context
) {

    BIFIT_LOG_DEBUG("return top element of operand stack\n");

    // pop the top element and
    bifit_object_reference_t *op = bifit_operand_stack_pop_reference(&(stack_frame->operand_stack));
    // push it onto the operand stack of the caller frame
    bifit_stack_frame_t *caller_frame = bifit_stack_peek_second(&(context->frame_stack));
    bifit_operand_stack_push_reference(&(caller_frame->operand_stack), op);

    return pc;
}