//
// Created by Roman Braunstingl on 13.06.23.
//

#ifndef BIFIT_BIFIT_GC_H
#define BIFIT_BIFIT_GC_H

#include "../common/bifit_common.h"

void bifit_gc_mark();

void bifit_gc_run() {
    bifit_gc_mark();
}

void bifit_gc_clear_all_marks(bifit_object_node_t *object_register) {
    bifit_object_node_t *p = object_register;
    while (p != NULL) {
        p->marked = false;
        p = p->next;
    }
}

void bifit_gc_mark_reachable_from_object_reference(bifit_object_reference_t *object_reference) {

    // currently there are no objects referencing each other, thus we can just mark the given object
    bifit_object_node_t *p = bifit_object_get_node(object_reference);
    p->marked = true;
}

void bifit_gc_mark_reachable_from_local_variables(bifit_stack_frame_t *stack_frame) {
    int number_of_local_vars = stack_frame->current_method->code.max_locals;
    for (int i = 0; i < number_of_local_vars; ++i) {
        bifit_object_reference_t *current_reference = stack_frame->local_variable_array[i].object_reference;
        bifit_gc_mark_reachable_from_object_reference(current_reference);
    }
}

void bifit_gc_mark_reachable_from_operand_stack(bifit_stack_frame_t *stack_frame) {

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

void bifit_gc_mark() {
    bifit_gc_clear_all_marks(bifit_object_register);
    bifit_gc_mark_reachable_from_stack(bifit_context);
}

#endif //BIFIT_BIFIT_GC_H
