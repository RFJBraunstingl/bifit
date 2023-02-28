#include "../common/bifit_interpreter_common.h"

unsigned int bifit_execute_instruction_dup(unsigned int pc, bifit_context_t *context) {
    LOG_DEBUG("duplicate the top operand stack value\n");

    bifit_stack_t *operand_stack = &(context->stack_frame->operand_stack);
    bifit_stack_element_t *top_element = bifit_stack_peek(operand_stack);
    bifit_stack_element_t *new_element = bifit_stack_duplicate_element(top_element);
    bifit_stack_push(operand_stack, new_element);

    return pc;
}