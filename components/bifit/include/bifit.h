#ifndef BIFIT_STD_H_
#define BIFIT_STD_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "string.h"

#include "../src/classloader/bifit_classloader.h"
#include "../src/interpreter/bifit_interpreter.h"
#include "../src/native/bifit_native.h"

bifit_method_t *bifit_find_main_method_in_class(bifit_class_t *clazz) {
    BIFIT_LOG_DEBUG("scanning for a (public static) main method...\n");

    unsigned int main_class_method_count = clazz->methods.method_count;
    bifit_method_t *main_class_methods = clazz->methods.method_array;

    for (int i = 0; i < main_class_method_count; ++i) {

        bifit_method_t current_method = main_class_methods[i];

        // main method has to be public
        if (!current_method.access_flags.is_public) {
            BIFIT_LOG_DEBUG("found method ");
            bifit_log_bifit_identifier(&current_method.name);
            BIFIT_LOG_DEBUG(" which is not public!\n");
            continue;
        }

        // main method has to be static
        if (!current_method.access_flags.is_static) {
            BIFIT_LOG_DEBUG("found method ");
            bifit_log_bifit_identifier(&current_method.name);
            BIFIT_LOG_DEBUG(" which is not static!\n");
            continue;
        }

        // main method has to be called "main"
        if (!bifit_identifier_matches_string(&current_method.name, "main")) {
            BIFIT_LOG_DEBUG("found method ");
            bifit_log_bifit_identifier(&current_method.name);
            BIFIT_LOG_DEBUG(" which is not called 'main'!\n");
            continue;
        }

        BIFIT_LOG_DEBUG("will use method with index %d as main\n\n", i);
        // main method found
        return &clazz->methods.method_array[i];
    }

    BIFIT_LOG_ERROR("could not find main method to execute!");
    exit(1);
}

void bifit_initialize_context() {
    bifit_context = malloc(sizeof(struct bifit_context));
    bifit_context->class_list = NULL;
    bifit_context->class_list_size = 0;
    bifit_context->resolved_static_references = NULL;

    bifit_native_bind(bifit_context);
}

void bifit_run() {

    bifit_initialize_context();
    bifit_load_embedded_classes(bifit_context);
    BIFIT_LOG_DEBUG("classes loaded!\n");

    bifit_class_t *main_class = bifit_find_class_by_name(
            bifit_context,
            bifit_main_class_identifier
    );

    BIFIT_LOG_DEBUG("\n\nmain class found: %s\n\n", bifit_main_class_identifier);

    // construct main frame
    bifit_interpreter_allocate_stack_frame(
            bifit_context,
            main_class,
            bifit_find_main_method_in_class(main_class)
    );

    BIFIT_LOG_DEBUG("start executing main...\n\n");
    bifit_interpreter_execute_current_stack_frame(bifit_context);
}

#endif