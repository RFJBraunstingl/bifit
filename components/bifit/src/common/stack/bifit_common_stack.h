#ifndef BIFIT_COMMON_STACK_H_
#define BIFIT_COMMON_STACK_H_

#include "../types/bifit_types.h"
#include "../bifit_defs.h"
#include <stdlib.h>

typedef struct bifit_stack_element {

    void *data;
    struct bifit_stack_element *next;
    struct bifit_stack_element *prev;

} bifit_stack_element_t;

typedef struct bifit_stack {

    bifit_stack_element_t *top;

} bifit_stack_t;

void bifit_stack_push(bifit_stack_t *stack, bifit_stack_element_t *element) {
    if (stack->top == NULL) {
        LOG_DEBUG("stack was empty - element is top now\n");
        stack->top = element;
        element->prev = NULL;
    } else {
        LOG_DEBUG("push element on top...\n");
        stack->top->next = element;
        element->prev = stack->top;
        stack->top = element;
    }
}

bifit_stack_element_t *bifit_stack_pop(bifit_stack_t *stack) {

    LOG_DEBUG("bifit_stack_pop\n");

    if (stack->top == NULL) {
        LOG_DEBUG("ERROR: stack was empty - can not pop\n");
        exit(1);
    } else {
        bifit_stack_element_t *ref = stack->top;
        stack->top = stack->top->prev;
        stack->top = NULL;
        return ref;
    }
}

bifit_stack_element_t *bifit_stack_peek(bifit_stack_t *stack) {
    return stack->top;
}

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


#endif