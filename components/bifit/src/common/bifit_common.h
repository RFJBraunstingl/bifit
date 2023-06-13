#ifndef BIFIT_COMMON_H_
#define BIFIT_COMMON_H_

#include "bifit_common_base.h"
#include "bifit_common_identifier.h"
#include "bifit_common_object.h"
#include "bifit_common_stack.h"

unsigned int bifit_parse_integer_u2(unsigned int index, const uint8_t data[]) {
    unsigned int result = data[index];
    result = result << 8;

    result += data[index + 1];

    return result;
}

int bifit_parse_integer_s2(unsigned int index, const uint8_t data[]) {
    int16_t result = data[index];
    result = result << 8;

    result |= data[index + 1];

    return result;
}

unsigned int bifit_parse_integer_u4(unsigned int index, const uint8_t data[]) {
    unsigned int result = data[index++];
    result = result << 8;

    result += data[index++];
    result = result << 8;

    result += data[index++];
    result = result << 8;

    result += data[index];

    return result;
}

bifit_class_t *bifit_find_class_by_name(bifit_context_t *context, char *identifier) {
    for (int i = 0; i < context->class_list_size; ++i) {
        if (bifit_identifier_matches_string(
                &(context->class_list[i].this_class),
                identifier
        )) {

            return &(context->class_list[i]);
        }
    }

    BIFIT_LOG_ERROR("class not found error: %s\n", identifier);
    exit(1);
}

bifit_class_t *bifit_find_class_by_identifier(bifit_context_t *context, bifit_identifier_t *identifier) {
    BIFIT_LOG_DEBUG("bifit_find_class_by_identifier ");
    bifit_log_bifit_identifier(identifier);
    BIFIT_LOG_DEBUG("\n");

    bifit_identifier_t *lookup_identifier = identifier;

    for (int i = 0; i < context->class_list_size; ++i) {
        if (bifit_identifier_matches_identifier(
                &(context->class_list[i].this_class),
                lookup_identifier)) {

            return &context->class_list[i];
        }
    }

    BIFIT_LOG_ERROR("class not found error: ");
    bifit_log_bifit_identifier(identifier);
    BIFIT_LOG_ERROR("\n");

    BIFIT_KERNEL_PANIC("class def not found error!");
}

bifit_object_reference_t *bifit_resolved_static_reference_get(
        bifit_context_t *context,
        bifit_identifier_t *class_identifier,
        bifit_identifier_t *field_identifier) {

    bifit_resolved_static_reference_node_t *p = context->resolved_static_references;
    while (p != NULL) {
        if (bifit_identifier_matches_identifier(p->class_identifier, class_identifier) &&
            bifit_identifier_matches_identifier(p->field_identifier, field_identifier)) {

            return p->reference;
        }

        p = p->next;
    }

    return NULL;
}

void bifit_resolved_static_reference_put(
        bifit_context_t *context,
        bifit_identifier_t *class_identifier,
        bifit_identifier_t *field_identifier,
        bifit_object_reference_t *reference
) {

    bifit_resolved_static_reference_node_t *new_node = malloc(sizeof(struct bifit_resolved_static_reference_node));
    new_node->class_identifier = class_identifier;
    new_node->field_identifier = field_identifier;
    new_node->reference = reference;
    new_node->next = NULL;

    bifit_resolved_static_reference_node_t *p = context->resolved_static_references;
    if (p == NULL) {
        context->resolved_static_references = new_node;
        return;
    }

    while (p->next != NULL) {
        p = p->next;
    }

    p->next = new_node;
}

#endif