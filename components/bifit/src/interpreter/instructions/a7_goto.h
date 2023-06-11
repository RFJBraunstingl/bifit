#include "../common/bifit_interpreter_common.h"

unsigned int bifit_execute_instruction_goto(unsigned int pc, bifit_stack_frame_t *stack_frame) {

    BIFIT_LOG_DEBUG("goto pc offset\n");

    const uint8_t *code = stack_frame->current_method->code.byte_code;
    int pc_offset = bifit_parse_integer_s2(++pc, code);

    return pc + pc_offset - 1;
}