#ifndef BIFIT_STD_H_
#define BIFIT_STD_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "string.h"

#include "../bifit_classloader/load_class.h"

bool bifit_identifier_matches_string(bifit_identifier_t *identifier, char *string) {
    LOG_DEBUG("identifier_matches_string checking identifier ");
    bifit_log_bifit_identifier(identifier);
    LOG_DEBUG("\n");
    if (identifier->identifier_length != strlen(string)) {
        return false;
    }

    for (int i = 0; i < identifier->identifier_length; ++i) {
        if ((identifier->identifier)[i] != string[i]) {
            return false;
        }
    }

    return true;
}

bifit_class_t *bifit_find_class_by_name(bifit_class_t *class_list, char *identifier) {
    for (int i = 0; i < bifit_embedded_class_files_size; ++i) {
        if (bifit_identifier_matches_string(&(class_list[i].this_class), identifier)) {
            return &class_list[i];
        }
    }

    return NULL;
}

void bifit_run() {

    bifit_class_t *class_list = bifit_load_embedded_classes();
    LOG_DEBUG("classes loaded!\n");

    bifit_class_t *main_class = bifit_find_class_by_name(class_list, bifit_main_class_identifier);
    if (main_class == NULL) {
        printf("ERROR: main class %s not found in loaded class_list!\n", bifit_main_class_identifier);
        exit(1);
    }
    LOG_DEBUG("main class found: %s\n", bifit_main_class_identifier);

    // bifit_execute_main_method_for_class();
    unsigned int main_class_method_count = main_class->methods.method_count;
    bifit_method_t *main_class_methods = main_class->methods.method_array;
    bifit_method_t *main_method_of_class;
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
        LOG_DEBUG("main method found...starting execution\n");
        main_method_of_class = &current_method;
    }
}

#endif