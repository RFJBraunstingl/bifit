#ifndef LOAD_CLASS_UTILS_H_
#define LOAD_CLASS_UTILS_H_

#include <stdlib.h>

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

/*
attribute_info {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 info[attribute_length];
}
*/
unsigned int bifit_load_attribute(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *entries, bifit_attribute_t *out) {

    unsigned int name_index = bifit_parse_integer_u2(index, data);
    LOG_DEBUG("name_index: %d\n", name_index);
    bifit_load_identifier_by_name_index(name_index, entries, &(out->name));
    index += 2;
    LOG_DEBUG("loading attribute ");
    bifit_log_bifit_identifier(&(out->name));
    LOG_DEBUG("\n");

    out->length_in_bytes = bifit_parse_integer_u4(index, data);
    index += 4;
    LOG_DEBUG("attribute length was %d\n", out->length_in_bytes);

    out->data = &data[index];

    return index + out->length_in_bytes;
}

#endif