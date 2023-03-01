#ifndef BIFIT_LOAD_CLASS_H_
#define BIFIT_LOAD_CLASS_H_

#include "class_header.h"
#include "constant_pool.h"
#include "class_access_flags.h"
#include "class_identifier.h"
#include "interfaces.h"
#include "fields.h"
#include "methods.h"

void bifit_load_class(const uint8_t *data, bifit_class_t *out) {

    bifit_load_class_header(data, &out->class_header);
    bifit_load_constant_pool(data, &out->constant_pool);

    unsigned int byte_index = BIFIT_CLASS_HEADER_SIZE_IN_BYTES + out->constant_pool.size_in_bytes;

    bifit_load_class_access_flags(byte_index, data, &out->access_flags);
    byte_index += BIFIT_CLASS_ACCESS_FLAGS_SIZE_IN_BYTES;

    bifit_load_class_identifier(byte_index, data, out->constant_pool.entries, &(out->this_class));
    byte_index += BIFIT_CLASS_IDENTIFIER_SIZE_IN_BYTES;
    bifit_load_class_identifier(byte_index, data, out->constant_pool.entries, &(out->super_class));
    byte_index += BIFIT_CLASS_IDENTIFIER_SIZE_IN_BYTES;

    bifit_load_interfaces(byte_index, data, out);
    byte_index += out->interfaces.size_in_bytes;

    bifit_load_fields(byte_index, data, out);
    byte_index += out->fields.size_in_bytes;

    bifit_load_methods(byte_index, data, out->constant_pool.entries, &(out->methods));
    byte_index += out->methods.size_in_bytes;

    LOG_DEBUG("after loading methods ");
    bifit_log_bifit_identifier(&(out->this_class));
    LOG_DEBUG("\n");
}


#endif