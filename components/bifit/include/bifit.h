#ifndef BIFIT_STD_H_
#define BIFIT_STD_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "string.h"

#include "../bifit_classloader/bifit_classloader.h"
#include "../bifit_interpreter/bifit_interpreter.h"

void bifit_find_main_method_in_class(bifit_class_t *clazz, bifit_stack_frame_t *out) {
    LOG_DEBUG("scanning for main method...\n");
    out->current_class = clazz;

    unsigned int main_class_method_count = clazz->methods.method_count;
    bifit_method_t *main_class_methods = clazz->methods.method_array;

    for (int i = 0; i < main_class_method_count; ++i) {

        bifit_method_t current_method = main_class_methods[i];

        // main method has to be public
        if (!current_method.access_flags.is_public) {
            LOG_DEBUG("found method ");
            bifit_log_bifit_identifier(&current_method.name);
            LOG_DEBUG(" which is not public!\n");
            continue;
        }

        // main method has to be static
        if (!current_method.access_flags.is_static) {
            LOG_DEBUG("found method ");
            bifit_log_bifit_identifier(&current_method.name);
            LOG_DEBUG(" which is not static!\n");
            continue;
        }

        // main method has to be called "main"
        if (!bifit_identifier_matches_string(&current_method.name, "main")) {
            LOG_DEBUG("found method ");
            bifit_log_bifit_identifier(&current_method.name);
            LOG_DEBUG(" which is not called 'main'!\n");
            continue;
        }

        // main method found
        out->current_method = &clazz->methods.method_array[i];
        return;
    }

    LOG_ERROR("could not find main method to execute!");
    exit(1);
}

void bifit_run() {

    bifit_class_t *class_list = bifit_load_embedded_classes();
    LOG_DEBUG("classes loaded!\n");
    bifit_context_t *context = malloc(sizeof(struct bifit_context));
    context->class_list = class_list;

    bifit_class_t *main_class = bifit_find_class_by_name(
            class_list,
            bifit_embedded_class_files_size,
            bifit_main_class_identifier
    );
    if (main_class == NULL) {
        printf("ERROR: main class %s not found in loaded class_list!\n", bifit_main_class_identifier);
        exit(1);
    }
    LOG_DEBUG("\n\nmain class found: %s\n\n", bifit_main_class_identifier);

    // construct main frame
    bifit_stack_frame_t *main_frame = malloc(sizeof(struct bifit_stack_frame));
    main_frame->bifit_context = context;
    bifit_find_main_method_in_class(main_class, main_frame);

    LOG_DEBUG("main method found!\n\n");
    bifit_execute_main_frame(main_frame);
}

#endif