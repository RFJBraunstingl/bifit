#ifndef BIFIT_INTERPRETER_H_
#define BIFIT_INTERPRETER_H_

#include "instructions/12_ldc.h"
#include "instructions/2a-2d_aload_n.h"
#include "instructions/4b-4e_astore_n.h"
#include "instructions/59_dup.h"
#include "instructions/a7_goto.h"
#include "instructions/b0_areturn.h"
#include "instructions/b2_getstatic.h"
#include "instructions/b6_invokevirtual.h"
#include "instructions/b7_invokespecial.h"
#include "instructions/b8_invokestatic.h"
#include "instructions/bb_new.h"

#ifdef BIFIT_CONFIG_DONT_DIE_ON_UNKNOWN_OPCODE
static bool bifit_dont_die_on_unknown_opcode = true;
#else
static bool bifit_dont_die_on_unknown_opcode = false;
#endif

void bifit_interpreter_execute_current_stack_frame(bifit_context_t *context) {
    BIFIT_LOG_DEBUG("bifit_execute_stack_frame\n");
    bifit_stack_frame_t *stack_frame = bifit_stack_peek(&(context->frame_stack));

    BIFIT_LOG_DEBUG("loading code\n");
    bifit_method_code_t method_code = stack_frame->current_method->code;

    BIFIT_LOG_DEBUG("executing method ");
    bifit_log_bifit_identifier(&(stack_frame->current_method->name));
    BIFIT_LOG_DEBUG("\n");

    const uint8_t *code = method_code.byte_code;
    for (unsigned int pc = 0; pc < method_code.byte_code_length; ++pc) {
        BIFIT_LOG_DEBUG("\n%03d: %02x\n", pc, code[pc]);

        switch (code[pc]) {

            case 0x00:
                // nop
                BIFIT_LOG_DEBUG("no op\n");
                break;

            case 0x12:
                pc = bifit_execute_instruction_ldc(pc, stack_frame);
                break;

            case 0x2a:
                pc = bifit_execute_instruction_aload_n(pc, stack_frame, 0);
                break;

            case 0x2b:
                pc = bifit_execute_instruction_aload_n(pc, stack_frame, 1);
                break;

            case 0x2c:
                pc = bifit_execute_instruction_aload_n(pc, stack_frame, 2);
                break;

            case 0x2d:
                pc = bifit_execute_instruction_aload_n(pc, stack_frame, 3);
                break;

            case 0x4b:
                pc = bifit_execute_instruction_astore_n(pc, stack_frame, 0);
                break;

            case 0x4c:
                pc = bifit_execute_instruction_astore_n(pc, stack_frame, 1);
                break;

            case 0x4d:
                pc = bifit_execute_instruction_astore_n(pc, stack_frame, 2);
                break;

            case 0x4e:
                pc = bifit_execute_instruction_astore_n(pc, stack_frame, 3);
                break;

            case 0x59:
                pc = bifit_execute_instruction_dup(pc, stack_frame);
                break;

            case 0xa7:
                pc = bifit_execute_instruction_goto(pc, stack_frame);
                break;

            case 0xb0:
                pc = bifit_execute_instruction_areturn(pc, stack_frame, context);
                break;

            case 0xb1:
                BIFIT_LOG_DEBUG("return\n");
                return;

            case 0xb2:
                pc = bifit_execute_instruction_getstatic(pc, stack_frame, context);
                break;

            case 0xb6:
                pc = bifit_execute_instruction_invokevirtual(pc, stack_frame, context);
                break;

            case 0xb7:
                pc = bifit_execute_instruction_invokespecial(pc, stack_frame, context);
                break;

            case 0xb8:
                pc = bifit_execute_instruction_invokestatic(pc, stack_frame, context);
                break;

            case 0xbb:
                pc = bifit_execute_instruction_new(pc, stack_frame, context);
                break;

            default:
                BIFIT_LOG_DEBUG("unknown op code\n");
                if (bifit_dont_die_on_unknown_opcode) {
                    break;
                }
                BIFIT_KERNEL_PANIC("unsupported opcode!")
        }
    }
    BIFIT_LOG_DEBUG("\n");
}

#endif