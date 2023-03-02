#include "../common/bifit_interpreter_common.h"

unsigned int bifit_execute_instruction_getstatic(unsigned int pc, bifit_stack_frame_t *stack_frame, bifit_context_t *context) {
    LOG_DEBUG("Get static field from class\n");

    const uint8_t *code = stack_frame->current_method->code.byte_code;
    unsigned int field_ref_entry = bifit_parse_integer_u2(++pc, code);
    LOG_DEBUG("const_pool_index: %d\n", field_ref_entry);

    /* i.e.:
     * const_pool_index == 4
     * ---
     * reading constant pool entry 4
     * constant has tag 9 (field ref)
     * class_index was 46
     * name and type index was 47
     *
     * reading constant pool entry 46
     * constant has tag 7 (class ref)
     * name_index was 64
     *
     * reading constant pool entry 47
     * constant has tag 12 (name and type constant)
     * name_index was 65
     * desc_index was 66
     *
     * reading constant pool entry 64
     * constant has tag 1 (UTF-8 constant)
     * length was 16
     * attempt to print utf8 as ascii: java/lang/System
     *
     * reading constant pool entry 65
     * constant has tag 1 (UTF-8 constant)
     * length was 3
     * attempt to print utf8 as ascii: out
     *
     * reading constant pool entry 66
     * constant has tag 1 (UTF-8 constant)
     * length was 21
     * attempt to print utf8 as ascii: Ljava/io/PrintStream;
     */

    // not implemented...

    return ++pc;
}