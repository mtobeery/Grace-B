#include <stdlib.h>
#include "../include/stack.h"

static stack_frame_t* top = NULL;

void push_frame(symbol_table_t* symbols) {
    stack_frame_t* frame = malloc(sizeof(stack_frame_t));
    frame->symbols = symbols;
    frame->next = top;
    top = frame;
}

void pop_frame() {
    if (top) {
        stack_frame_t* temp = top;
        top = top->next;
        free_symbol_table(temp->symbols);
        free(temp);
    }
}

symbol_table_t* current_symbols() {
    return top ? top->symbols : NULL;
}

void clear_stack() {
    while (top) pop_frame();
}
