#include "../common/bifit_interpreter_common.h"

unsigned int bifit_execute_instruction_dup(unsigned int pc, bifit_stack_frame_t *stack_frame) {
    BIFIT_LOG_DEBUG("duplicate the top operand stack value\n");

    bifit_stack_t *operand_stack = &(stack_frame->operand_stack);
    void *top_element = bifit_operand_stack_pop_reference(operand_stack);
    bifit_operand_stack_push_reference(operand_stack, top_element);
    bifit_operand_stack_push_reference(operand_stack, top_element);

    return pc;
}