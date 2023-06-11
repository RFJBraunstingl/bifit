#ifndef BIFIT_COMMON_STACK_H_
#define BIFIT_COMMON_STACK_H_

#include "types/bifit_types.h"
#include "bifit_common_base.h"
#include <stdlib.h>

typedef struct bifit_stack_element {

    void *data;
    struct bifit_stack_element *next;
    struct bifit_stack_element *prev;

} bifit_stack_element_t;

typedef struct bifit_stack {

    bifit_stack_element_t *top;

} bifit_stack_t;

bifit_stack_element_t *bifit_stack_create_element_with_data(void *data) {
    bifit_stack_element_t *element = malloc(sizeof(struct bifit_stack_element));

    element->prev = NULL;
    element->next = NULL;
    element->data = data;

    return element;
}

bifit_stack_element_t *bifit_stack_create_element() {
    return bifit_stack_create_element_with_data(NULL);
}

bifit_stack_element_t *bifit_stack_duplicate_element(bifit_stack_element_t *template) {
    return bifit_stack_create_element_with_data(template->data);
}

void bifit_stack_push(bifit_stack_t *stack, void *data) {
    bifit_stack_element_t *element = bifit_stack_create_element_with_data(data);
    if (stack->top == NULL) {
        BIFIT_LOG_DEBUG("stack was empty - element is top now\n");
        stack->top = element;
        element->prev = NULL;
    } else {
        BIFIT_LOG_DEBUG("push element on top...\n");
        stack->top->next = element;
        element->prev = stack->top;
        stack->top = element;
    }
}

void *bifit_stack_pop(bifit_stack_t *stack) {

    BIFIT_LOG_DEBUG("bifit_stack_pop\n");

    if (stack->top == NULL) {
        BIFIT_KERNEL_PANIC("ERROR: stack was empty - can not pop\n")
    } else {
        bifit_stack_element_t *ref = stack->top;
        stack->top = stack->top->prev;

        if (stack->top != NULL) {
            stack->top->next = NULL;
        }

        return ref->data;
    }
}

void *bifit_stack_peek(bifit_stack_t *stack) {
    return stack->top->data;
}

void *bifit_stack_peek_second(bifit_stack_t *stack) {
    return stack->top->prev->data;
}

#endif