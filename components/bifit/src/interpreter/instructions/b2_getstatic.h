#include "../common/bifit_interpreter_common.h"

unsigned int
bifit_execute_instruction_getstatic(
        unsigned int pc,
        bifit_stack_frame_t *stack_frame,
        bifit_context_t *context) {

    LOG_DEBUG("Get static field from class\n");

    const uint8_t *code = stack_frame->current_method->code.byte_code;
    unsigned int field_ref_index = bifit_parse_integer_u2(++pc, code);
    LOG_DEBUG("const_pool_index: %d\n", field_ref_index);
    ++pc;

    /*
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

    bifit_constant_pool_entry_t field_ref_entry =
            stack_frame->current_class->constant_pool.entries[field_ref_index - 1];
    LOG_DEBUG("class_index was %d\n", field_ref_entry.class_index);
    LOG_DEBUG("name_and_type_index was %d\n", field_ref_entry.name_and_type_index);

    bifit_constant_pool_entry_t class_ref_entry =
            stack_frame->current_class->constant_pool.entries[field_ref_entry.class_index - 1];

    bifit_identifier_t class_identifier;
    bifit_load_identifier_by_name_index(
            class_ref_entry.name_index,
            stack_frame->current_class->constant_pool.entries,
            &class_identifier
    );
    LOG_DEBUG("attempt to getstatic from class ");
    bifit_log_bifit_identifier(&class_identifier);
    LOG_DEBUG("\n");

    bifit_constant_pool_entry_t name_and_type_entry =
            stack_frame->current_class->constant_pool.entries[field_ref_entry.name_and_type_index - 1];

    bifit_identifier_t field_identifier;
    bifit_load_identifier_by_name_index(
            name_and_type_entry.name_index,
            stack_frame->current_class->constant_pool.entries,
            &field_identifier
    );
    LOG_DEBUG("attempt to getstatic fieldname: ");
    bifit_log_bifit_identifier(&field_identifier);
    LOG_DEBUG("\n");

    /* check if the static field is already resolved */
    bifit_object_reference_t *reference = bifit_resolved_static_reference_get(
            context,
            &class_identifier,
            &field_identifier
    );
    if (reference != NULL) {
        bifit_stack_push(&(stack_frame->operand_stack), reference);
        return pc;
    }

    bifit_class_t *src_class = bifit_find_class_by_identifier(
            context,
            &class_identifier
    );
    for (int i = 0; i < src_class->fields.field_count; ++i) {
        bifit_field_t current_field = src_class->fields.field_array[i];
        if (bifit_identifier_matches_identifier(
                &current_field.name,
                &field_identifier
        )) {
            LOG_DEBUG("field by name was found!\n");
            if (!current_field.access_flags.is_static) {
                LOG_DEBUG("it was NOT static, though...\n");
                continue;
            }

            // TODO: push the field found on the operand stack
        }
    }

    return pc;
}