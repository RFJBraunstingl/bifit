#ifndef BIFIT_INTERPRETER_H_
#define BIFIT_INTERPRETER_H_

#include "instructions/dup.h"
#include "instructions/invokespecial.h"
#include "instructions/new.h"

void bifit_execute_current_stack_frame_in_context(bifit_context_t *context) {
    LOG_DEBUG("bifit_execute_main_frame\n");

    LOG_DEBUG("gather method\n");
    bifit_method_code_t method_code = context->stack_frame->current_method->code;

    LOG_DEBUG("main_frame got method ");
    bifit_log_bifit_identifier(&(context->stack_frame->current_method->name));
    LOG_DEBUG("\n");

    const uint8_t *code = method_code.byte_code;
    for (unsigned int pc = 0; pc < method_code.byte_code_length; ++pc) {
        LOG_DEBUG("%03d: %02x\n", pc, code[pc]);

        switch (code[pc]) {

            case 0x59:
                pc = bifit_execute_instruction_dup(pc, context);
                break;

            case 0xb7:
                pc = bifit_execute_instruction_invokespecial(pc, context);
                break;

            case 0xbb:
                pc = bifit_execute_instruction_new(pc, context);
                break;

            default:
                LOG_DEBUG("unknown op code\n");
                break;
        }
    }
    LOG_DEBUG("\n");
}

#endif