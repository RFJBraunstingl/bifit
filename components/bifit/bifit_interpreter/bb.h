#include "../bifit_common/bifit_common.h"

unsigned int bifit_execute_instruction_bb(unsigned int pc, bifit_stack_frame_t *main_frame) {
    LOG_DEBUG("create new object\n");
    const uint8_t *code = main_frame->current_method->code.byte_code;

    // consume next 2 instructions as index to constant pool
    unsigned int const_pool_index = bifit_parse_integer_u2(++pc, code);
    LOG_DEBUG("const_pool_index: %d\n", const_pool_index);

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


    return ++pc;
}