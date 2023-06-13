//
// Created by Roman Braunstingl on 13.06.23.
//

#ifndef BIFIT_BIFIT_GC_MARK_H
#define BIFIT_BIFIT_GC_MARK_H

#include "bifit_object.h"

void bifit_gc_clear_all_marks(bifit_object_node_t *object_register) {
    bifit_object_node_t *p = object_register;
    while (p != NULL) {
        p->marked = false;
        p = p->next;
    }
}

bifit_object_node_t *bifit_object_get_node(bifit_object_reference_t *reference) {
    bifit_object_node_t *p = bifit_object_register;
    while (p != NULL) {
        if (p->object->reference.id == reference->id) {
            return p;
        }

        p = p->next;
    }

    return NULL;
}

void bifit_gc_mark_reachable_from_object_reference(bifit_object_reference_t *object_reference) {

    if (object_reference == NULL) {
        return;
    }

    // currently there are no objects referencing each other, thus we can just mark the given object
    bifit_object_node_t *p = bifit_object_register;
    while (p != NULL) {
        if (p->object->reference.id == object_reference->id) {
            p->marked = true;
        }

        p = p->next;
    }
}

void bifit_gc_mark_reachable_from_local_variables(bifit_stack_frame_t *stack_frame) {
    unsigned int number_of_local_vars = stack_frame->current_method->code.max_locals;
    for (int i = 0; i < number_of_local_vars; ++i) {
        bifit_object_reference_t *current_reference = stack_frame->local_variable_array[i].object_reference;
        bifit_gc_mark_reachable_from_object_reference(current_reference);
    }
}

void bifit_gc_mark_reachable_from_operand(void *data) {
    bifit_operand_t *operand = data;
    bifit_gc_mark_reachable_from_object_reference(operand->object_reference);
}

void bifit_gc_mark_reachable_from_operand_stack(bifit_stack_frame_t *stack_frame) {
    bifit_stack_traverse_top_to_bottom(
            &(stack_frame->operand_stack),
            &(bifit_gc_mark_reachable_from_operand)
    );
}

void bifit_gc_mark_reachable_from_stack_frame(void *p) {
    bifit_stack_frame_t *stack_frame = p;
    bifit_gc_mark_reachable_from_local_variables(stack_frame);
    bifit_gc_mark_reachable_from_operand_stack(stack_frame);
}

void bifit_gc_mark_reachable_from_stack(bifit_context_t *context) {
    bifit_stack_traverse_top_to_bottom(
            &(context->frame_stack),
            &(bifit_gc_mark_reachable_from_stack_frame)
    );
}

void bifit_gc_mark_static_vars(bifit_context_t *context) {
    bifit_resolved_static_reference_node_t *p = context->resolved_static_references;
    while (p != NULL) {
        bifit_gc_mark_reachable_from_object_reference(p->reference);
        p = p->next;
    }
}

void bifit_gc_mark() {
    bifit_gc_clear_all_marks(bifit_object_register);
    bifit_gc_mark_reachable_from_stack(bifit_context);
    bifit_gc_mark_static_vars(bifit_context);
}

#endif //BIFIT_BIFIT_GC_MARK_H
