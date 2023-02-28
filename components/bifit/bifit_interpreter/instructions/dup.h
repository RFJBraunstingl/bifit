#include "../bifit_interpreter_common.h"

unsigned int bifit_execute_instruction_dup(unsigned int pc, bifit_stack_frame_t *stack_frame) {
    LOG_DEBUG("duplicate the top operand stack value\n");

    bifit_operand_stack_t *operand_stack = &(stack_frame->operand_stack);
    bifit_operand_stack_element_t *top_element = bifit_operand_stack_peek(operand_stack);
    bifit_operand_stack_element_t *new_element = bifit_operand_stack_duplicate_element(top_element);
    bifit_operand_stack_push(operand_stack, new_element);

    return pc;
}