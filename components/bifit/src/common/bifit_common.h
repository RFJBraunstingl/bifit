#ifndef BIFIT_COMMON_H_
#define BIFIT_COMMON_H_

#include "bifit_defs.h"
#include "types/bifit_types.h"
#include "stack/bifit_common_stack.h"

#include <stdio.h>
#include <string.h>

unsigned int bifit_parse_integer_u2(unsigned int index, const uint8_t data[]) {
    unsigned int result = data[index];
    result = result << 8;

    result += data[index + 1];

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

void bifit_log_identifier_with_length(const uint8_t *identifier, unsigned int identifier_length) {
    for (int i = 0; i < identifier_length; ++i) {
        LOG_DEBUG("%c", identifier[i]);
    }
}

void bifit_load_identifier_by_name_index(unsigned int name_index, bifit_constant_pool_entry_t *entries,
                                         bifit_identifier_t *out) {
    // NOTE: indexing in constant pool starts with 1!
    bifit_constant_pool_entry_t identifier_entry = entries[name_index - 1];

    // TODO: check if actually utf8 constant?
    (out->identifier) = identifier_entry.utf8_str;
    (out->identifier_length) = identifier_entry.utf8_str_len;
}

void bifit_log_bifit_identifier(bifit_identifier_t *identifier) {
    bifit_log_identifier_with_length(identifier->identifier, identifier->identifier_length);
}

void bifit_copy_identifier(bifit_identifier_t *src, bifit_identifier_t *out) {
    out->identifier_length = src->identifier_length;
    out->identifier = src->identifier;
}

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

bifit_class_t *bifit_find_class_by_name(bifit_context_t *context, char *identifier) {
    for (int i = 0; i < context->class_list_size; ++i) {
        if (bifit_identifier_matches_string(
                &(context->class_list[i].this_class),
                identifier
        )) {

            return &(context->class_list[i]);
        }
    }

    LOG_ERROR("class not found error: %s\n", identifier);
    exit(1);
}

bool bifit_identifier_matches_identifier(bifit_identifier_t *left, bifit_identifier_t *right) {
    if (left->identifier_length != right->identifier_length) {
        return false;
    }

    for (int i = 0; i < left->identifier_length; ++i) {
        if ((left->identifier)[i] != (right->identifier)[i]) {
            return false;
        }
    }

    return true;
}

bifit_class_t *bifit_find_class_by_identifier(bifit_context_t *context, bifit_identifier_t *identifier) {
    LOG_DEBUG("bifit_find_class_by_identifier ");
    bifit_log_bifit_identifier(identifier);
    LOG_DEBUG("\n");

    bifit_identifier_t *lookup_identifier = identifier;

    // prefix 'java/' is ignored
    // (thus we can inject our own system classes)
    if (identifier->identifier_length > 5 &&
        'j' == identifier->identifier[0] &&
        'a' == identifier->identifier[1] &&
        'v' == identifier->identifier[2] &&
        'a' == identifier->identifier[3] &&
        '/' == identifier->identifier[4]) {

        LOG_DEBUG("find class special case - java/ prefix is ignored!\n");
        bifit_identifier_t *temp = malloc(sizeof(struct bifit_identifier));
        bifit_copy_identifier(identifier, temp);

        temp->identifier = &temp->identifier[5];
        temp->identifier_length -= 5;

        bifit_class_t *result = bifit_find_class_by_identifier(context, temp);
        free(temp);

        return result;

    } else {

        for (int i = 0; i < context->class_list_size; ++i) {
            if (bifit_identifier_matches_identifier(
                    &(context->class_list[i].this_class),
                    lookup_identifier)) {

                return &context->class_list[i];
            }
        }

        LOG_ERROR("class not found error: ");
        bifit_log_bifit_identifier(identifier);
        LOG_ERROR("\n");

        exit(1);
    }
}

#endif