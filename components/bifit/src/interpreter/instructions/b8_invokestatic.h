#include "../common/bifit_interpreter_common.h"
#include "../../native/bifit_native.h"

unsigned int bifit_execute_instruction_invokestatic(
        unsigned int pc,
        bifit_stack_frame_t *stack_frame,
        bifit_context_t *context) {

    BIFIT_LOG_DEBUG("invoke static method\n");

    const uint8_t *code = stack_frame->current_method->code.byte_code;
    unsigned int const_pool_index = bifit_parse_integer_u2(++pc, code);
    ++pc;

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

    BIFIT_LOG_DEBUG("invokestatic - should invoke ");
    bifit_log_bifit_identifier(&class_identifier);
    BIFIT_LOG_DEBUG(".");
    bifit_log_bifit_identifier(&method_identifier);
    BIFIT_LOG_DEBUG("\n");

    // check if method is native
    if (bifit_native_execute_method(&class_identifier, &method_identifier, context)) {
        // if return == true, the method was invoked
        return pc;
    }

    // otherwise, we continue to look for the java implementation
    bifit_class_t *bifit_class = bifit_find_class_by_identifier(
            context,
            &class_identifier
    );

    bifit_method_t *bifit_method = NULL;
    for (int i = 0; i < bifit_class->methods.method_count; ++i) {
        bifit_identifier_t current_method_name = bifit_class->methods.method_array[i].name;
        if (bifit_identifier_matches_identifier(&method_identifier, &current_method_name)) {
            bifit_method = &(bifit_class->methods.method_array[i]);
            break;
        }
    }

    if (bifit_method == NULL) {
        BIFIT_LOG_DEBUG("could not find method by this name: ");
        bifit_log_bifit_identifier(&method_identifier);
        BIFIT_LOG_DEBUG("\n");
        BIFIT_KERNEL_PANIC("no such method error!");
    }

    bifit_stack_frame_t *invoked_stack_frame = bifit_interpreter_allocate_stack_frame(context, bifit_class,
                                                                                      bifit_method);

    uint8_t number_of_arguments = bifit_interpreter_count_arguments(&(bifit_method->descriptor));
    for (int i = (number_of_arguments - 1); i >= 0; --i) {
        // note: currently we only support object references
        bifit_object_reference_t *reference = bifit_operand_stack_pop_reference(&(stack_frame->operand_stack));
        invoked_stack_frame->local_variable_array[i].object_reference = reference;
    }

    bifit_interpreter_execute_current_stack_frame(context);

    // after execution of the new stack frame, free all resources
    bifit_interpreter_deallocate_current_stack_frame(context);

    return pc;
}