//
// Created by Roman Braunstingl on 10.06.23.
//

#ifndef BIFIT_NATIVE_CLASS_PRINTSTREAM_H
#define BIFIT_NATIVE_CLASS_PRINTSTREAM_H

#include "bifit_native_classes_common.h"
#include "./String.h"

void bifit_native_PrintStream_println(bifit_context_t *context) {
    bifit_stack_frame_t *stack_frame = bifit_stack_peek(&(context->frame_stack));
    bifit_object_reference_t *string_reference = bifit_operand_stack_pop_reference(&(stack_frame->operand_stack));

    // TODO: validate stream is known
    // bifit_object_reference_t *stream_reference = bifit_operand_stack_pop_reference(&(stack_frame->operand_stack));

    bifit_object_t *string_object = bifit_object_get(string_reference);
    bifit_native_String_t *native_string = string_object->meta;

    for (int i = 0; i < native_string->len; ++i) {
        printf("%c", native_string->str[i]);
    }
    printf("\n");
}

bifit_native_class_t *bifit_native_class_PrintStream_init() {
    bifit_identifier_t *identifier = bifit_identifier_from_string(
            "java/io/PrintStream",
            19
    );

    bifit_native_class_t *clazz = malloc(sizeof(struct bifit_native_class));
    clazz->identifier = identifier;
    clazz->methods = NULL;

    bifit_native_class_register_method(
            clazz,
            bifit_identifier_from_string("println", 7),
            &bifit_native_PrintStream_println
    );

    return clazz;
}

#endif //BIFIT_PRINTSTREAM_H
