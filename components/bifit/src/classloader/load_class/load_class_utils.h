#ifndef LOAD_CLASS_UTILS_H_
#define LOAD_CLASS_UTILS_H_

#include <stdlib.h>

/*
attribute_info {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 info[attribute_length];
}
*/
unsigned int bifit_load_attribute(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *entries, bifit_attribute_t *out) {

    unsigned int name_index = bifit_parse_integer_u2(index, data);
    BIFIT_LOG_DEBUG("name_index: %d\n", name_index);
    bifit_load_identifier_by_name_index(name_index, entries, &(out->name));
    index += 2;
    BIFIT_LOG_DEBUG("loading attribute ");
    bifit_log_bifit_identifier(&(out->name));
    BIFIT_LOG_DEBUG("\n");

    out->length_in_bytes = bifit_parse_integer_u4(index, data);
    index += 4;
    BIFIT_LOG_DEBUG("attribute length was %d\n", out->length_in_bytes);

    out->data = &data[index];

    return index + out->length_in_bytes;
}

#endif