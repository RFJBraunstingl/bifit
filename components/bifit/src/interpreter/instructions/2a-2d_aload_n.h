#include "../common/bifit_interpreter_common.h"

unsigned int bifit_execute_instruction_aload_n(unsigned int pc, bifit_stack_frame_t *stack_frame, int n) {
    LOG_DEBUG("load reference from local variable\n");

    bifit_stack_element_t *top_operand_element = bifit_stack_pop(&(stack_frame->operand_stack));
    bifit_operand_t *top_operand = top_operand_element->data;
    stack_frame->local_variable_array[n].object_reference = top_operand->object_reference;

    return pc;
}