#ifndef STACK_H
#define STACK_H

#include "symbol_table.h"

typedef struct stack_frame {
    symbol_table_t* symbols;
    struct stack_frame* next;
} stack_frame_t;

void push_frame(symbol_table_t* symbols);
void pop_frame();
symbol_table_t* current_symbols();
void clear_stack();

#endif // STACK_H
