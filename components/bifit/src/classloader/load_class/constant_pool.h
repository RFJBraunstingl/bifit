#include "../../common/bifit_common.h"

unsigned int bifit_load_next_constant_pool_entry(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out);
unsigned int bifit_load_next_utf8(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out);
unsigned int bifit_load_next_integer(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out);
unsigned int bifit_load_next_float(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out);
unsigned int bifit_load_next_long(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out);
unsigned int bifit_load_next_double(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out);
unsigned int bifit_load_next_class_ref(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out);
unsigned int bifit_load_next_string(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out);
unsigned int bifit_load_next_ref(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out);
unsigned int bifit_load_next_field_ref(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out);
unsigned int bifit_load_next_method_ref(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out);
unsigned int bifit_load_next_iface_method_ref(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out);
unsigned int bifit_load_next_name_and_type(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out);
unsigned int bifit_load_next_method_handle(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out);


#define BIFIT_CONSTANT_POOL_START_INDEX 8

void bifit_load_constant_pool(const uint8_t data[], bifit_constant_pool_t *out) {

    unsigned int const_pool_size = bifit_parse_integer_u2(BIFIT_CONSTANT_POOL_START_INDEX, data);
    unsigned int number_of_entries = const_pool_size - 1;
    BIFIT_LOG_DEBUG("\nstart to parse constant pool of size %d\n", number_of_entries);

    out->entries = malloc(
            sizeof(struct bifit_constant_pool_entry) * number_of_entries
    );
    BIFIT_DEBUG_GC("malloc 4 %p\n", out->entries);
    out->entry_count = number_of_entries;

    int byte_index = 10;
    for (int i = 1; i < const_pool_size; i++) {
        BIFIT_LOG_DEBUG("reading constant pool entry %d\n", i);
        byte_index = bifit_load_next_constant_pool_entry(
                byte_index,
                data,
                &(out->entries[i - 1])
        );
        BIFIT_LOG_DEBUG("\n");
    }

    out->size_in_bytes = byte_index - BIFIT_CONSTANT_POOL_START_INDEX;
}

unsigned int bifit_load_next_constant_pool_entry(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out) {
    uint8_t tag = data[index++];
    out->type = tag;
    BIFIT_LOG_DEBUG("constant has tag %d ", tag);

    switch (tag) {

        case 1:
            return bifit_load_next_utf8(index, data, out);

        case 3:
            return bifit_load_next_integer(index, data, out);

        case 4:
            return bifit_load_next_float(index, data, out);

        case 5:
            return bifit_load_next_long(index, data, out);

        case 6:
            return bifit_load_next_double(index, data, out);

        case 7:
            return bifit_load_next_class_ref(index, data, out);

        case 8:
            return bifit_load_next_string(index, data, out);

        case 9:
            return bifit_load_next_field_ref(index, data, out);

        case 10:
            return bifit_load_next_method_ref(index, data, out);

        case 11:
            return bifit_load_next_iface_method_ref(index, data, out);

        case 12:
            return bifit_load_next_name_and_type(index, data, out);

        case 15:
            return bifit_load_next_method_handle(index, data, out);

        default:
            return index;
    }
}

/*
CONSTANT_Utf8_info {
    u1 tag;
    u2 length;
    u1 bytes[length];
}
*/
unsigned int bifit_load_next_utf8(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out) {
    BIFIT_LOG_DEBUG("(UTF-8 constant)\n");
    int length = bifit_parse_integer_u2(index, data);
    index += 2;
    BIFIT_LOG_DEBUG("length was %d\n", length);

    out->utf8_str = &data[index];
    out->utf8_str_len = length;

    BIFIT_LOG_DEBUG("attempt to print utf8 as ascii: ");
    for (int i = 0; i < out->utf8_str_len; i++) {
        BIFIT_LOG_DEBUG("%c", out->utf8_str[i]);
    }
    BIFIT_LOG_DEBUG("\n");

    return index + length;
}

/*
CONSTANT_Integer_info {
    u1 tag;
    u4 bytes;
}
 */
unsigned int bifit_load_next_integer(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out) {

    BIFIT_LOG_DEBUG("(int)\n");

    uint8_t bytes[4];
    bytes[3] = data[index++];
    bytes[2] = data[index++];
    bytes[1] = data[index++];
    bytes[0] = data[index++];

    out->long_value = *(int *) bytes;
    BIFIT_LOG_DEBUG("integer was %ld\n", out->long_value);

    return index;
}

/*
CONSTANT_Float_info {
    u1 tag;
    u4 bytes;
}
*/
unsigned int bifit_load_next_float(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out) {

    BIFIT_LOG_DEBUG("(float)\n");

    uint8_t bytes[4];
    bytes[3] = data[index++];
    bytes[2] = data[index++];
    bytes[1] = data[index++];
    bytes[0] = data[index++];

    out->double_value = *(float *) bytes;
    BIFIT_LOG_DEBUG("float was %f\n", out->double_value);

    return index;
}

/*
CONSTANT_Long_info {
    u1 tag;
    u4 high_bytes;
    u4 low_bytes;
}
*/
unsigned int bifit_load_next_long(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out) {

    BIFIT_LOG_DEBUG("(long)\n");

    uint8_t bytes[8];
    bytes[7] = data[index++];
    bytes[6] = data[index++];
    bytes[5] = data[index++];
    bytes[4] = data[index++];
    bytes[3] = data[index++];
    bytes[2] = data[index++];
    bytes[1] = data[index++];
    bytes[0] = data[index++];

    out->long_value = *(long *) bytes;
    BIFIT_LOG_DEBUG("long was %ld\n", out->long_value);

    return index;
}

/*
CONSTANT_Double_info {
    u1 tag;
    u4 high_bytes;
    u4 low_bytes;
}
*/
unsigned int bifit_load_next_double(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out) {

    BIFIT_LOG_DEBUG("(double)\n");

    uint8_t bytes[8];
    bytes[7] = data[index++];
    bytes[6] = data[index++];
    bytes[5] = data[index++];
    bytes[4] = data[index++];
    bytes[3] = data[index++];
    bytes[2] = data[index++];
    bytes[1] = data[index++];
    bytes[0] = data[index++];

    out->double_value = *(double *) bytes;
    BIFIT_LOG_DEBUG("double was %f\n", out->double_value);

    return index;
}

/*
CONSTANT_Class_info {
    u1 tag;
    u2 name_index;
}
 */
unsigned int bifit_load_next_class_ref(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out) {

    BIFIT_LOG_DEBUG("(class ref)\n");

    out->name_index = bifit_parse_integer_u2(index, data);
    BIFIT_LOG_DEBUG("name_index was %d\n", out->name_index);

    return index + 2;
}

/*
CONSTANT_String_info {
    u1 tag;
    u2 string_index;
}
*/
unsigned int bifit_load_next_string(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out) {

    BIFIT_LOG_DEBUG("(string)\n");

    out->name_index = bifit_parse_integer_u2(index, data);
    BIFIT_LOG_DEBUG("string_index was %d\n", out->name_index);

    return index + 2;
}

/*
CONSTANT_???ref_info {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
}
*/
unsigned int bifit_load_next_ref(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out) {
    out->class_index = bifit_parse_integer_u2(index, data);
    index += 2;
    BIFIT_LOG_DEBUG("class_index was %d\n", out->class_index);

    out->name_and_type_index = bifit_parse_integer_u2(index, data);
    index += 2;
    BIFIT_LOG_DEBUG("name and type index was %d\n", out->name_and_type_index);

    return index;
}

/*
CONSTANT_Fieldref_info {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
}
*/
unsigned int bifit_load_next_field_ref(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out) {
    BIFIT_LOG_DEBUG("(field ref)\n");
    return bifit_load_next_ref(index, data, out);
}

/*
CONSTANT_Methodref_info {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
}
*/
unsigned int bifit_load_next_method_ref(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out) {
    BIFIT_LOG_DEBUG("(method ref)\n");
    return bifit_load_next_ref(index, data, out);
}

/*
CONSTANT_InterfaceMethodref_info {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
}
*/
unsigned int bifit_load_next_iface_method_ref(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out) {
    BIFIT_LOG_DEBUG("(interface method ref)\n");
    return bifit_load_next_ref(index, data, out);
}

/*
CONSTANT_NameAndType_info {
    u1 tag;
    u2 name_index;
    u2 descriptor_index;
}
*/
unsigned int bifit_load_next_name_and_type(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out) {

    BIFIT_LOG_DEBUG("(name and type constant)\n");

    out->name_index = bifit_parse_integer_u2(index, data);
    index += 2;
    BIFIT_LOG_DEBUG("name_index was %d\n", out->name_index);

    out->desc_index = bifit_parse_integer_u2(index, data);
    index += 2;
    BIFIT_LOG_DEBUG("desc_index was %d\n", out->desc_index);

    return index;
}

/*
CONSTANT_MethodHandle_info {
    u1 tag;
    u1 reference_kind;
    u2 reference_index;
}
*/
unsigned int bifit_load_next_method_handle(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t *out) {

    BIFIT_LOG_DEBUG("(method handle)\n");

    out->ref_type = data[index++];
    BIFIT_LOG_DEBUG("reference_kind was %d\n", out->ref_type);

    out->ref_index = bifit_parse_integer_u2(index, data);
    BIFIT_LOG_DEBUG("reference_index was %d\n", out->ref_index);

    return index + 2;
}