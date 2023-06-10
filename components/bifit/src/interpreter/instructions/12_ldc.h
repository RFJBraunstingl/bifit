#include "../common/bifit_interpreter_common.h"

#include "../../native/bifit_native.h"

unsigned int bifit_execute_instruction_ldc(unsigned int pc, bifit_stack_frame_t *stack_frame) {
    LOG_DEBUG("push single byte indexed constant pool entry onto operand stack\n");

    const uint8_t *code = stack_frame->current_method->code.byte_code;
    const uint8_t const_pool_index = code[++pc];

    /* e.g.:
     * const_pool_index = 2
     * ---
     * reading constant pool entry 2
     * constant has tag 8 (string)
     * string_index was 17
     *
     * reading constant pool entry 17
     * constant has tag 1 (UTF-8 constant)
     * length was 12
     * attempt to print utf8 as ascii: Hello world!
     */

    bifit_constant_pool_entry_t constant_entry =
            stack_frame->current_class->constant_pool.entries[const_pool_index - 1];

    if (constant_entry.type != CONSTANT_POOL_TAG_STRING) {
        KERNEL_PANIC("ldc: constant type not supported!")
    }

    unsigned int string_index = constant_entry.name_index;
    bifit_constant_pool_entry_t utf8_entry =
            stack_frame->current_class->constant_pool.entries[string_index - 1];

    bifit_object_t *string_object = bifit_create_object();
    string_object->meta = bifit_native_class_String_wrap(
            utf8_entry.utf8_str,
            utf8_entry.utf8_str_len
    );

    bifit_operand_stack_push_reference(
            &stack_frame->operand_stack,
            &string_object->reference
    );

    return pc;
}