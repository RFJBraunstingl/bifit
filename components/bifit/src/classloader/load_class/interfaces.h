#include "../../common/bifit_common.h"

void bifit_load_interfaces(unsigned int start_index, const uint8_t data[], bifit_class_t *out) {
    BIFIT_LOG_DEBUG("load_interfaces\n");
    unsigned int byte_index = start_index;

    out->interfaces.interface_count = bifit_parse_integer_u2(byte_index, data);
    BIFIT_LOG_DEBUG("interface_count: %d\n", out->interfaces.interface_count);
    byte_index += 2;

    out->interfaces.identifiers = malloc(
            sizeof(struct bifit_identifier) * out->interfaces.interface_count
    );

    for (int i = 0; i < out->interfaces.interface_count; ++i) {
        bifit_load_class_identifier(
                byte_index,
                data,
                out->constant_pool.entries,
                &out->interfaces.identifiers[i]
        );
        byte_index += 2;
    }

    out->interfaces.size_in_bytes = byte_index - start_index;
    BIFIT_LOG_DEBUG("\n");
}