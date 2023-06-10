//
// Created by Roman Braunstingl on 10.06.23.
//

#ifndef BIFIT_BIFIT_COMMON_IDENTIFIER_H
#define BIFIT_BIFIT_COMMON_IDENTIFIER_H

#include "bifit_common_base.h"

void bifit_log_identifier_with_length(const uint8_t *identifier, unsigned int identifier_length) {
    for (int i = 0; i < identifier_length; ++i) {
        LOG_DEBUG("%c", identifier[i]);
    }
}

void bifit_load_identifier_by_name_index(unsigned int name_index, bifit_constant_pool_entry_t *entries,
                                         bifit_identifier_t *out) {
    // NOTE: indexing in constant pool starts with 1!
    bifit_constant_pool_entry_t identifier_entry = entries[name_index - 1];

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

bifit_identifier_t *bifit_identifier_from_string(const char *str, int len) {

    uint8_t *identifier = malloc(sizeof(uint8_t) * len);
    memcpy(identifier, str, len);

    bifit_identifier_t *result = malloc(sizeof(struct bifit_identifier));
    result->identifier = identifier;
    result->identifier_length = len;

    return result;
}

#endif //BIFIT_BIFIT_COMMON_IDENTIFIER_H
