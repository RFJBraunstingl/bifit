#define LOG_DEBUG printf
#define LOG_ERROR printf

#ifndef BIFIT_COMMON_H_
#define BIFIT_COMMON_H_

#include "bifit_types.h"

#include <string.h>

unsigned int bifit_parse_integer_u2(unsigned int index, const uint8_t data[]) {
    unsigned int result = data[index];
    result  = result << 8;

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

void bifit_load_identifier_by_name_index(unsigned int name_index, bifit_constant_pool_entry_t *entries, bifit_identifier_t *out) {
    // NOTE: indexing in constant pool starts with 1!
    bifit_constant_pool_entry_t identifier_entry = entries[name_index - 1];
    LOG_DEBUG("load_identifier_by_name_index ");
    bifit_log_identifier_with_length(identifier_entry.utf8_str, identifier_entry.utf8_str_len);
    LOG_DEBUG("\n");

    // TODO: check if actually utf8 constant?
    (out->identifier) = identifier_entry.utf8_str;
    (out->identifier_length) = identifier_entry.utf8_str_len;
}

void bifit_log_bifit_identifier(bifit_identifier_t *identifier) {
    bifit_log_identifier_with_length(identifier->identifier, identifier->identifier_length);
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

#endif