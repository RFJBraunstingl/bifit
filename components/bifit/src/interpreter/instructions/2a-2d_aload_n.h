#include "../common/bifit_interpreter_common.h"

unsigned int bifit_execute_instruction_aload_n(unsigned int pc, bifit_stack_frame_t *stack_frame, int n) {
    BIFIT_LOG_DEBUG("load reference from local variable\n");

    bifit_object_reference_t *ref = stack_frame->local_variable_array[n].object_reference;

    bifit_stack_t *operand_stack = &(stack_frame->operand_stack);
    bifit_operand_stack_push_reference(operand_stack, ref);

    return pc;
}