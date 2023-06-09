#include "../common/bifit_interpreter_common.h"

unsigned int
bifit_execute_instruction_new(unsigned int pc, bifit_stack_frame_t *stack_frame, bifit_context_t *context) {
    LOG_DEBUG("create new object\n");
    const uint8_t *code = stack_frame->current_method->code.byte_code;

    // consume next 2 instructions as index to constant pool
    unsigned int const_pool_index = bifit_parse_integer_u2(++pc, code);
    LOG_DEBUG("const_pool_index: %d\n", const_pool_index);
    ++pc;

    /*
     * const_pool_index == 2
     * ---
     * reading constant pool entry 2
     * constant has tag 7 (class ref)
     * name_index was 45
     *
     * reading constant pool entry 45
     * constant has tag 1 (UTF-8 constant)
     * length was 13
     * attempt to print utf8 as ascii: hello/Greeter
     */
    bifit_constant_pool_entry_t class_reference_entry =
            stack_frame->current_class->constant_pool.entries[const_pool_index - 1];

    bifit_identifier_t class_identifier; // = malloc(sizeof(struct bifit_identifier));
    bifit_load_identifier_by_name_index(
            class_reference_entry.name_index,
            stack_frame->current_class->constant_pool.entries,
            &class_identifier
    );

    LOG_DEBUG("attempt to create object of type ");
    bifit_log_bifit_identifier(&class_identifier);
    LOG_DEBUG("\n");

    bifit_class_t *bifit_class = bifit_find_class_by_identifier(
            context,
            &class_identifier
    );

    LOG_DEBUG("create new object reference for class\n");
    bifit_object_t *new_object = bifit_create_object();
    new_object->object_class = bifit_class;

    LOG_DEBUG("push reference onto operand stack\n");
    bifit_push_reference_onto_operand_stack(
            &(new_object->reference),
            &(stack_frame->operand_stack)
    );

    return pc;
}