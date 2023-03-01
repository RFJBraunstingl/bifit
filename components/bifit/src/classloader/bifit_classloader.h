#ifndef BIFIT_CLASS_LOADER_H_
#define BIFIT_CLASS_LOADER_H_

#include "../common/bifit_common.h"
#include "load_class/load_class.h"

#ifdef DEPLOY_TO_ESP
#include "classes_combined.h"
#else
#include "../../../classes_combined/include/classes_combined.h"
#endif

void bifit_load_embedded_classes(bifit_context_t *context) {

    unsigned int number_of_classes = bifit_embedded_class_files_size;

    bifit_class_t *classes = malloc(sizeof(struct bifit_class) * number_of_classes);

    for (int i = 0; i < number_of_classes; i++) {
        uint8_t *class_file = bifit_embedded_class_files[i];
        bifit_load_class(class_file, &classes[i]);
    }

    context->class_list = classes;
    context->class_list_size = number_of_classes;
}

#endif
