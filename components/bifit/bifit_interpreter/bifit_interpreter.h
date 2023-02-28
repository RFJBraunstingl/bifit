#ifndef BIFIT_INTERPRETER_H_
#define BIFIT_INTERPRETER_H_

#include "../bifit_common.h"

void bifit_execute_main_frame(bifit_stack_frame_t *main_frame) {
    LOG_DEBUG("bifit_execute_main_frame\n");
    bifit_method_code_t method_code = main_frame->current_method->code;

    LOG_DEBUG("main_frame got method ");
    bifit_log_bifit_identifier(&main_frame->current_method->name);
    LOG_DEBUG("\n");

    const uint8_t *code = method_code.byte_code;
    for (unsigned int pc = 0; pc < method_code.byte_code_length; ++pc) {
        LOG_DEBUG("%03d: %02x\n", pc, code[pc]);
    }
    LOG_DEBUG("\n");
}

#endif