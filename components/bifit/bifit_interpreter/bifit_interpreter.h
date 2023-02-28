#ifndef BIFIT_INTERPRETER_H_
#define BIFIT_INTERPRETER_H_

#include "../bifit_common/bifit_common.h"

void bifit_execute_main_frame(bifit_stack_frame_t *main_frame) {
    LOG_DEBUG("bifit_execute_main_frame\n");
    bifit_method_code_t method_code = main_frame->current_method->code;

    LOG_DEBUG("main_frame got method ");
    bifit_log_bifit_identifier(&main_frame->current_method->name);
    LOG_DEBUG("\n");

    const uint8_t *code = method_code.byte_code;
    for (unsigned int pc = 0; pc < method_code.byte_code_length; ++pc) {
        LOG_DEBUG("%03d: %02x\n", pc, code[pc]);

        switch (code[pc]) {

            case 0xbb:
                LOG_DEBUG("create new object\n");

                // consume next 2 instructions as index to constant pool
                unsigned int const_pool_index = bifit_parse_integer_u2(++pc, code);
                LOG_DEBUG("const_pool_index: %d\n", const_pool_index);
                ++pc;

                /* i.e.:
                 * reading constant pool entry 2
                 * constant has tag 7 (class ref)
                 * name_index was 45
                 * ---
                 * reading constant pool entry 45
                 * constant has tag 1 (UTF-8 constant)
                 * length was 13
                 * attempt to print utf8 as ascii: hello/Greeter
                 */


                break;

            default:
                LOG_DEBUG("unknown op code\n");
                break;
        }
    }
    LOG_DEBUG("\n");
}

#endif