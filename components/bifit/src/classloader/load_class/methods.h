#ifndef BIFIT_LOAD_CLASS_METHODS_H_
#define BIFIT_LOAD_CLASS_METHODS_H_

#include "../../common/bifit_common.h"
#include "load_class_utils.h"

unsigned int bifit_load_method(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t entries[], bifit_method_t *out);
unsigned int bifit_load_method_access_flags(unsigned int index, const uint8_t *data, bifit_method_access_flags_t *out);
void bifit_load_method_code(bifit_method_t *method);

void bifit_load_methods(unsigned int start_index, const uint8_t *data, bifit_constant_pool_entry_t entries[], bifit_methods_t *out) {

    unsigned int index = start_index;

    out->method_count = bifit_parse_integer_u2(index, data);
    BIFIT_LOG_DEBUG("load_methods num of methods: %d\n", out->method_count);
    index += 2;

    out->method_array = malloc(sizeof(struct bifit_method) * out->method_count);
    BIFIT_DEBUG_GC("malloc 6 %p\n", out->method_array);

    for (int i = 0; i < out->method_count; ++i) {
        index = bifit_load_method(index, data, entries, &(out->method_array[i]));
    }

    out->size_in_bytes = start_index - index;
}

/*
method_info {
    u2             access_flags;
    u2             name_index;
    u2             descriptor_index;
    u2             attributes_count;
    attribute_info attributes[attributes_count];
}
*/
unsigned int bifit_load_method(unsigned int index, const uint8_t *data, bifit_constant_pool_entry_t entries[], bifit_method_t *out) {

    index = bifit_load_method_access_flags(index, data, &(out->access_flags));

    unsigned int name_index = bifit_parse_integer_u2(index, data);
    index += 2;

    BIFIT_LOG_DEBUG("load_method name_index was %d\n", name_index);
    bifit_load_identifier_by_name_index(name_index, entries, &(out->name));

    BIFIT_LOG_DEBUG("loading method ");
    bifit_log_bifit_identifier(&(out->name));
    BIFIT_LOG_DEBUG("\n");

    unsigned int descriptor_index = bifit_parse_integer_u2(index, data);
    index += 2;
    bifit_load_identifier_by_name_index(descriptor_index, entries, &(out->descriptor));

    BIFIT_LOG_DEBUG("descriptor ");
    bifit_log_bifit_identifier(&(out->descriptor));
    BIFIT_LOG_DEBUG("\n");


    out->attributes_count = bifit_parse_integer_u2(index, data);
    BIFIT_LOG_DEBUG("method attribute count was %d\n", out->attributes_count);
    index += 2;

    // TODO: unify with field attributes
    out->attributes = malloc(sizeof(struct bifit_attribute) * out->attributes_count);
    BIFIT_DEBUG_GC("malloc 7 %p\n", out->attributes);

    for (int i = 0; i < out->attributes_count; ++i) {
        index = bifit_load_attribute(index, data, entries, &(out->attributes[i]));
    }

    bifit_load_method_code(out);

    BIFIT_LOG_DEBUG("\n");
    return index;
}

unsigned int bifit_load_method_access_flags(unsigned int index, const uint8_t *data, bifit_method_access_flags_t *out) {

    uint8_t msb = data[index];
    uint8_t lsb = data[index + 1];

    out->is_public = (0x01 & lsb) ? true : false;
    out->is_private = (0x02 & lsb) ? true : false;
    out->is_protected = (0x04 & lsb) ? true : false;
    out->is_static = (0x08 & lsb) ? true : false;
    out->is_final = (0x10 & lsb) ? true : false;
    out->is_synchronized = (0x20 & lsb) ? true : false;
    out->is_bridge = (0x40 & lsb) ? true : false;
    out->is_varargs = (0x80 & lsb) ? true : false;
    out->is_native = (0x01 & msb) ? true : false;
    out->is_abstract = (0x04 & msb) ? true : false;
    out->is_strict = (0x08 & msb) ? true : false;
    out->is_synthetic = (0x10 & msb) ? true : false;

    return index + 2;
}

/*
Code_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 max_stack;
    u2 max_locals;
    u4 code_length;
    u1 code[code_length];
    u2 exception_table_length;
    {   u2 start_pc;
        u2 end_pc;
        u2 handler_pc;
        u2 catch_type;
    } exception_table[exception_table_length];
    u2 attributes_count;
    attribute_info attributes[attributes_count];
}
*/
int bifit_is_code_attribute_identifier(bifit_identifier_t *identifier) {
    return identifier->identifier_length == 4 &&
           identifier->identifier[0] == 'C' &&
           identifier->identifier[1] == 'o' &&
           identifier->identifier[2] == 'd' &&
           identifier->identifier[3] == 'e';
}

void bifit_load_method_code(bifit_method_t *method) {

    bifit_method_code_t *out = &method->code;

    for (int i = 0; i < method->attributes_count; ++i) {
        bifit_attribute_t attr = method->attributes[i];

        if (bifit_is_code_attribute_identifier(&(attr.name))) {
            out->max_stack = bifit_parse_integer_u2(0, attr.data);
            BIFIT_LOG_DEBUG("method max_stack: %d\n", out->max_stack);
            out->max_locals = bifit_parse_integer_u2(2, attr.data);
            BIFIT_LOG_DEBUG("method max_locals: %d\n", out->max_locals);
            out->byte_code_length = bifit_parse_integer_u4(4, attr.data);
            BIFIT_LOG_DEBUG("code_length: %d\n", out->byte_code_length);
            out->byte_code = &attr.data[8];

            #ifdef BIFIT_CONFIG_PRINT_METHOD_BYTE_CODE
            for (int j = 0; j < out->byte_code_length; ++j) {
                BIFIT_LOG_DEBUG("0x%02x\n", out->byte_code[j]);
            }
            #endif

            unsigned int attr_data_index = 8 + out->byte_code_length;
            BIFIT_LOG_DEBUG("attr_data_index: %d\n", attr_data_index);

            // exception table loading follows here
        }
    }
}

#endif
