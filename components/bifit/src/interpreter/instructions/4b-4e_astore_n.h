#include "../common/bifit_interpreter_common.h"

unsigned int bifit_execute_instruction_astore_n(unsigned int pc, bifit_stack_frame_t *stack_frame, int n) {
    BIFIT_LOG_DEBUG("Store reference into local variable\n");

    bifit_operand_t *top_operand = bifit_stack_pop(&(stack_frame->operand_stack));
    stack_frame->local_variable_array[n].object_reference = top_operand->object_reference;

    return pc;
}