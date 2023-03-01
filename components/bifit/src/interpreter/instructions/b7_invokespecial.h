#include "../common/bifit_interpreter_common.h"

bool bifit_execute_instruction_invokespecial_should_call_superclass() {
    /*
     * If all of the following are true, let C be the direct superclass of the current class:
     * - The resolved method is not an instance initialization method (§2.9.1).
     * - If the symbolic reference names a class (not an interface), then that class is a superclass of the current class.
     * - The ACC_SUPER flag is set for the class file (§4.1).
     */
    return false; // TODO
}

unsigned int
bifit_execute_instruction_invokespecial(unsigned int pc, bifit_stack_frame_t *stack_frame, bifit_context_t *context) {
    LOG_DEBUG("invoke instance method\n");

    const uint8_t *code = stack_frame->current_method->code.byte_code;
    unsigned int const_pool_index = bifit_parse_integer_u2(++pc, code);
    ++pc;

    /* i.e.:
     * const_pool_index == 3
     * ---
     * reading constant pool entry 2
     * constant has tag 7 (class ref)
     * name_index was 45
     *
     * reading constant pool entry 3
     * constant has tag 10 (method ref)
     * class_index was 2
     * name and type index was 44
     *
     * reading constant pool entry 19
     * constant has tag 1 (UTF-8 constant)
     * length was 6
     * attempt to print utf8 as ascii: <init>
     *
     * reading constant pool entry 20
     * constant has tag 1 (UTF-8 constant)
     * length was 3
     * attempt to print utf8 as ascii: ()V
     *
     * reading constant pool entry 44
     * constant has tag 12 (name and type constant)
     * name_index was 19
     * desc_index was 20
     *
     * reading constant pool entry 45
     * constant has tag 1 (UTF-8 constant)
     * length was 13
     * attempt to print utf8 as ascii: hello/Greeter
     */

    bifit_constant_pool_entry_t method_reference_entry =
            stack_frame->current_class->constant_pool.entries[const_pool_index - 1];
    bifit_constant_pool_entry_t class_reference_entry =
            stack_frame->current_class->constant_pool.entries[method_reference_entry.class_index - 1];

    bifit_identifier_t class_identifier;
    bifit_load_identifier_by_name_index(
            class_reference_entry.name_index,
            stack_frame->current_class->constant_pool.entries,
            &class_identifier
    );

    bifit_constant_pool_entry_t name_and_type_entry =
            stack_frame->current_class->constant_pool.entries[method_reference_entry.name_and_type_index - 1];

    bifit_identifier_t method_identifier;
    bifit_load_identifier_by_name_index(
            name_and_type_entry.name_index,
            stack_frame->current_class->constant_pool.entries,
            &method_identifier
    );

    if (bifit_identifier_matches_string(&class_identifier, "java/lang/Object") &&
        bifit_identifier_matches_string(&method_identifier, "<init>")) {

        LOG_DEBUG("special case: constructor of java/lang/Object is no-op\n");
        return pc;
    }

    LOG_DEBUG("invokespecial - should invoke ");
    bifit_log_bifit_identifier(&class_identifier);
    LOG_DEBUG(".");
    bifit_log_bifit_identifier(&method_identifier);
    LOG_DEBUG("\n");

    if (bifit_execute_instruction_invokespecial_should_call_superclass()) {
        // ...
    }

    bifit_class_t *bifit_class = bifit_find_class_by_identifier(
            context,
            &class_identifier
    );

    bifit_method_t *bifit_method = NULL;
    for (int i = 0; i < bifit_class->methods.method_count; ++i) {
        if (!bifit_identifier_matches_identifier(
                &(method_identifier),
                &(bifit_class->methods.method_array[i].name)
        )) {
            continue;
        }

        bifit_method = &(bifit_class->methods.method_array[i]);
    }

    bifit_stack_frame_t *invoked_stack_frame = bifit_allocate_stack_frame(context, bifit_class, bifit_method);

    bifit_stack_element_t *top_operand_element = bifit_stack_pop(&(stack_frame->operand_stack));
    bifit_operand_t *top_operand = top_operand_element->data;
    bifit_object_reference_t *obj_ref = top_operand->object_reference;

    stack_frame->local_variable_array[0].object_reference = obj_ref;

    bifit_stack_element_t *frame_stack_element = bifit_stack_create_element_with_data(invoked_stack_frame);
    bifit_stack_push(&(context->frame_stack),frame_stack_element);
    bifit_execute_current_stack_frame_in_context(context);

    // free popped operand stack element
    free(top_operand_element);

    return pc;
}