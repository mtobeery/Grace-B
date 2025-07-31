#include <stdlib.h>
#include <string.h>
#include "../include/symbol_table.h"

static FunctionSymbol* func_head = NULL;

symbol_table_t* create_symbol_table() {
    symbol_table_t* t = malloc(sizeof(symbol_table_t));
    if (t) t->head = NULL;
    return t;
}

void free_symbol_table(symbol_table_t* table) {
    if (!table) return;
    Symbol* cur = table->head;
    while (cur) {
        Symbol* tmp = cur;
        cur = cur->next;
        free(tmp->name);
        free(tmp->value);
        free(tmp);
    }
    free(table);
}

void add_symbol(symbol_table_t* table, const char* name, const char* value) {
    if (!table) return;
    Symbol* s = malloc(sizeof(Symbol));
    if (!s) return;
    s->name = strdup(name);
    s->value = strdup(value);
    s->next = table->head;
    table->head = s;
}

const char* lookup_symbol(symbol_table_t* table, const char* name) {
    if (!table) return NULL;
    Symbol* cur = table->head;
    while (cur) {
        if (strcmp(cur->name, name) == 0) return cur->value;
        cur = cur->next;
    }
    return NULL;
}

void add_function(const char* name, ASTNode* decl) {
    FunctionSymbol* f = malloc(sizeof(FunctionSymbol));
    if (!f) return;
    f->name = strdup(name);
    f->declaration = decl;
    f->next = func_head;
    func_head = f;
}

FunctionSymbol* lookup_function(const char* name) {
    FunctionSymbol* cur = func_head;
    while (cur) {
        if (strcmp(cur->name, name) == 0) return cur;
        cur = cur->next;
    }
    return NULL;
}

void clear_functions() {
    FunctionSymbol* cur = func_head;
    while (cur) {
        FunctionSymbol* tmp = cur;
        cur = cur->next;
        free(tmp->name);
        free(tmp);
    }
    func_head = NULL;
}
